#include "../includes/Interpreter.hpp"
#include "../includes/Logger.hpp"
#include "../includes/messages.h"
#include "../includes/Parser/ast.h"

void Interpreter::interpret(std::vector<t_ast_node *> ast)
{
	for (auto node : ast)
	{
		switch (node->type)
		{
			case t_ast_node_type::AST_NODE_TYPE_ASSIGN:
				interpretAssignStatement((t_ast_node_assign *)node);
				break;
			case t_ast_node_type::AST_NODE_TYPE_WRITE:
				interpretWriteStatement((t_ast_node_write *)node);
				break;
			default:
				Logger::getInstance().log(LogLevel::ERROR, MSG_INVALID_AST_NODE_TYPE((int)node->type));
				exit(1);
		}
	}
}

Value Interpreter::evaluateExpression(t_ast_node *node)
{
    switch (node->type)
    {
        case t_ast_node_type::AST_NODE_TYPE_LITERAL:
        {
            t_ast_node_literal *literalNode = static_cast<t_ast_node_literal *>(node);
            if (literalNode->valueType == "int")
            {
                return Value(literalNode->intValue);
            }
            else if (literalNode->valueType == "string")
            {
                return Value(literalNode->stringValue);
            }
            else
            {
                Logger::getInstance().log(LogLevel::ERROR, "Unknown literal type");
                exit(1);
            }
        }
        case t_ast_node_type::AST_NODE_TYPE_VARIABLE:
        {
            t_ast_node_variable *varNode = static_cast<t_ast_node_variable *>(node);
            return context.getVariable(varNode->varName);
        }
        case t_ast_node_type::AST_NODE_TYPE_EXPR:
        {
            t_ast_node_expr *exprNode = static_cast<t_ast_node_expr *>(node);
            Value leftValue = evaluateExpression(exprNode->left);
            Value rightValue = evaluateExpression(exprNode->right);

            if (exprNode->op == '+')
            {
                if (leftValue.valueType == "int" && rightValue.valueType == "int")
                {
                    return Value(leftValue.intValue + rightValue.intValue);
                }
                else if (leftValue.valueType == "string" && rightValue.valueType == "string")
                {
                    return Value(leftValue.stringValue + rightValue.stringValue);
                }
				else if (leftValue.valueType == "string" && rightValue.valueType == "int")
				{
					return Value(leftValue.stringValue + std::to_string(rightValue.intValue));
				}
				else if (leftValue.valueType == "int" && rightValue.valueType == "string")
				{
					return Value(std::to_string(leftValue.intValue) + rightValue.stringValue);
				}
                else
                {
                    Logger::getInstance().log(LogLevel::ERROR, "Type mismatch in '+' operation");
                    exit(1);
                }
            }
			else if (exprNode->op == '-')
			{
				if (leftValue.valueType == "int" && rightValue.valueType == "int")
				{
					return Value(leftValue.intValue - rightValue.intValue);
				}
				else if (leftValue.valueType == "string" && rightValue.valueType == "string")
				{
					std::string::size_type pos = leftValue.stringValue.find(rightValue.stringValue);
					if (pos != std::string::npos)
					{
						return Value(leftValue.stringValue.substr(0, pos) + leftValue.stringValue.substr(pos + rightValue.stringValue.length()));
					}
					else
					{
						return leftValue;
					}
				}
				else
				{
					Logger::getInstance().log(LogLevel::ERROR, "Type mismatch in '-' operation");
					exit(1);
				}
			}
			else if (exprNode->op == '*')
			{
				if (leftValue.valueType == "int" && rightValue.valueType == "int")
				{
					return Value(leftValue.intValue * rightValue.intValue);
				}
				else if (leftValue.valueType == "string" && rightValue.valueType == "int")
				{
					std::string result = "";
					for (int i = 0; i < rightValue.intValue; i++)
					{
						result += leftValue.stringValue;
					}
					return Value(result);
				}
				else if (leftValue.valueType == "int" && rightValue.valueType == "string")
				{
					std::string result = "";
					for (int i = 0; i < leftValue.intValue; i++)
					{
						result += rightValue.stringValue;
					}
					return Value(result);
				}
				else
				{
					Logger::getInstance().log(LogLevel::ERROR, "Type mismatch in '*' operation");
					exit(1);
				}
			}
			else if (exprNode->op == '/')
			{
				if (leftValue.valueType == "int" && rightValue.valueType == "int")
				{
					return Value(leftValue.intValue / rightValue.intValue);
				}

				Logger::getInstance().log(LogLevel::ERROR, "Type mismatch in '/' operation");
				exit(1);
			}
            else
            {
                Logger::getInstance().log(LogLevel::ERROR, "Unsupported operator: " + std::string(1, exprNode->op));
                exit(1);
            }
        }
        default:
            Logger::getInstance().log(LogLevel::ERROR, "Unsupported AST node type in expression");
            exit(1);
    }
}



void Interpreter::interpretAssignStatement(t_ast_node_assign *node)
{
    Value value = evaluateExpression(node->expr);

    if (node->varType == value.valueType)
    {
        context.setVariable(node->varName, value);
    }
    else
    {
        Logger::getInstance().log(LogLevel::ERROR, "Type mismatch in assignment to variable '" + node->varName + "'");
        exit(1);
    }
}

void Interpreter::interpretWriteStatement(t_ast_node_write *node)
{
	Value value = evaluateExpression(node->expr);
	if (node->writeType == 0)
		std::cout << value.toString();
	else
		std::cout << value.toString() << std::endl;
}
