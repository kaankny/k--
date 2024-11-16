#include "../includes/Interpreter.hpp"
#include "../includes/Logger.hpp"
#include "../includes/messages.h"
#include "../includes/Parser/ast.h"
#include "../includes/Lexer/token.h"

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
			case t_ast_node_type::AST_NODE_TYPE_IF:
				interpretIfStatement((t_ast_node_if *)node);
				break;
			case t_ast_node_type::AST_NODE_TYPE_WHILE:
				interpretWhileStatement((t_ast_node_while *)node);
				break;
			case t_ast_node_type::AST_NODE_TYPE_FOR:
				interpretForStatement((t_ast_node_for *)node);
				break;
			case t_ast_node_type::AST_NODE_TYPE_READ:
				interpretReadStatement((t_ast_node_read *)node);
				break;
			case t_ast_node_type::AST_NODE_TYPE_FUNCTION:
				interpretFunctionDefinition((t_ast_node_function *)node);
				break;
			case t_ast_node_type::AST_NODE_TYPE_CALL:
				interpretFunctionCall((t_ast_node_call *)node);
				break;
			case t_ast_node_type::AST_NODE_TYPE_SWITCH:
				interpretSwitchStatement((t_ast_node_switch *)node);
				break;
			case t_ast_node_type::AST_NODE_TYPE_RETURN:
				throw evaluateExpression(((t_ast_node_return *)node)->returnValue);
			case t_ast_node_type::AST_NODE_TYPE_BREAK:
				throw std::string("break");
			case t_ast_node_type::AST_NODE_TYPE_CONTINUE:
				throw std::string("continue");
			default:
				Logger::getInstance().log(LogLevel::ERROR, MSG_INVALID_AST_NODE_TYPE((int)node->type));
				exit(1);
		}
	}
}

void Interpreter::interpretSwitchStatement(t_ast_node_switch *node)
{
    Value switchValue = evaluateExpression(node->switchExpr);

    bool caseMatched = false;

    for (auto &casePair : node->cases)
    {
        Value caseValue = evaluateExpression(casePair.first);

        if (caseValue.valueType != switchValue.valueType)
        {
            Logger::getInstance().log(LogLevel::ERROR, "Type mismatch in switch-case comparison");
            exit(1);
        }

        if (caseValue == switchValue || caseMatched)
        {
            caseMatched = true; // Eşleşen bir case bulundu
            try
            {
                interpret(casePair.second);
            }
            catch (const std::string &msg)
            {
                if (msg == "break")
                {
                    return; // `break` gelirse switch'ten çık
                }
                else
                {
                    throw; // Diğer hatalar yeniden fırlatılır
                }
            }
        }
    }

    if (!caseMatched && !node->defaultCase.empty())
    {
        // Hiçbir case eşleşmediyse varsayılan body’yi çalıştır
        interpret(node->defaultCase);
    }
}


Value Interpreter::interpretFunctionCall(t_ast_node_call *node)
{
	if (functions.find(node->functionName) == functions.end()) {
		Logger::getInstance().log(LogLevel::ERROR, "Function not found: " + node->functionName);
		exit(1);
	}

    t_ast_node_function *function = functions[node->functionName];

    if (node->arguments.size() != function->parameters.size()) {
        Logger::getInstance().log(LogLevel::ERROR, "Argument count mismatch for function: " + node->functionName);
        exit(1);
    }

    ScopeManager::getInstance().beginScope(); // Yeni bir kapsam başlat

    // Parametreleri değerlendir ve yerel olarak ata
    for (size_t i = 0; i < function->parameters.size(); i++) {
        Value argValue = evaluateExpression(node->arguments[i]);
         ScopeManager::getInstance().setVariable(function->parameters[i].first, argValue);
    }

    try {
        interpret(function->functionBody);
    } catch (Value returnValue) { // return ifadesinden gelen değeri yakala
        ScopeManager::getInstance().endScope();

		if (returnValue.valueType == "undefined")
		{
			Logger::getInstance().log(LogLevel::ERROR, "Undefined return value in function: " + node->functionName);
			exit(1);
		}
		else if (returnValue.valueType != function->returnType)
		{
			Logger::getInstance().log(LogLevel::ERROR, "Return type mismatch in function: " + node->functionName);
			exit(1);
		}
        return returnValue;
    }

     ScopeManager::getInstance().endScope();

    return Value(); // Eğer `return` ifadesi yoksa varsayılan olarak boş döndür
}


void Interpreter::interpretFunctionDefinition(t_ast_node_function *node)
{
	if (functions.find(node->functionName) != functions.end())
	{
		Logger::getInstance().log(LogLevel::ERROR, "Function already defined: " + node->functionName);
		exit(1);
	}
	functions[node->functionName] = node;
}

void Interpreter::interpretReadStatement(t_ast_node_read *node)
{
	std::string varName = node->varName;
	Value value =  ScopeManager::getInstance().getVariable(varName);
	if (value.valueType == "undefined")
	{
		Logger::getInstance().log(LogLevel::ERROR, "Variable '" + varName + "' is not defined");
		exit(1);
	}
	std::string varType = value.valueType;
	std::string input;
	std::cin >> input;

	if (varType == "int")
	{
		int intValue;
		try
		{
			intValue = std::stoi(input);
		}
		catch (const std::invalid_argument &e)
		{
			Logger::getInstance().log(LogLevel::ERROR, "Invalid input for integer variable '" + varName + "'");
			exit(1);
		}
		 ScopeManager::getInstance().setVariable(varName, Value(intValue));
	}
	else if (varType == "string")
	{
		 ScopeManager::getInstance().setVariable(varName, Value(input));
	}
	else if (varType == "bool")
	{
		if (input == "true")
		{
			 ScopeManager::getInstance().setVariable(varName, Value(true));
		}
		else if (input == "false")
		{
			 ScopeManager::getInstance().setVariable(varName, Value(false));
		}
		else
		{
			Logger::getInstance().log(LogLevel::ERROR, "Invalid input for boolean variable '" + varName + "'");
			exit(1);
		}
	}
	else
	{
		Logger::getInstance().log(LogLevel::ERROR, "Invalid variable type in read statement");
		exit(1);
	}
}

void Interpreter::interpretWhileStatement(t_ast_node_while *node)
{
    while (true)
	{
        Value conditionValue = evaluateExpression(node->condition);
        if (conditionValue.valueType != "bool")
		{
            Logger::getInstance().log(LogLevel::ERROR, "While statement condition must be a boolean expression");
            exit(1);
        }

        if (!conditionValue.boolValue)
		{
            break;
        }

        try
		{
            interpret(node->whileBody);
        } catch (const std::string &msg)
		{
			if (msg == "continue")
			{
				continue; // Continue ifadesi yakalanırsa döngünün bir sonraki adımına geç
			}
			else if (msg == "break")
			{
                break;  // Break ifadesi yakalanırsa döngüden çık
            }
			else
			{
                throw;  // Diğer hatalar yeniden fırlatılır
            }
        }
    }
}

void Interpreter::interpretForStatement(t_ast_node_for *node)
{
    if (node->init != nullptr)
	{
        interpretAssignStatement((t_ast_node_assign *)node->init);
    }

    while (true)
	{
        if (node->condition != nullptr)
		{
            Value conditionValue = evaluateExpression(node->condition);
            if (conditionValue.valueType != "bool")
			{
                Logger::getInstance().log(LogLevel::ERROR, "For statement condition must be a boolean expression");
                exit(1);
            }

            if (!conditionValue.boolValue)
			{
                break;
            }
        }

        try {
            interpret(node->forBody);
        } catch (const char *msg)
		{
			if (std::string(msg) == "continue")
			{
				if (node->update != nullptr)
				{
					interpretAssignStatement((t_ast_node_assign *)node->update);
				}
				continue; // Continue ifadesi yakalanırsa döngünün bir sonraki adımına geç
			}
            if (std::string(msg) == "break")
			{
                break;  // Break ifadesi yakalanırsa döngüden çık
            }
			else 
			{
                throw;  // Diğer hatalar yeniden fırlatılır
            }
        }

        if (node->update != nullptr) {
            interpretAssignStatement((t_ast_node_assign *)node->update);
        }
    }
}


void Interpreter::interpretIfStatement(t_ast_node_if *node)
{
	Value conditionValue = evaluateExpression(node->condition);
	if (conditionValue.valueType != "bool")
	{
		Logger::getInstance().log(LogLevel::ERROR, "If statement condition must be a boolean expression");
		exit(1);
	}

	if (conditionValue.boolValue)
	{
		interpret(node->ifBody);
	}
	else
	{
		for (auto elif : node->elifBodies)
		{
			Value elifConditionValue = evaluateExpression(elif.first);
			if (elifConditionValue.valueType != "bool")
			{
				Logger::getInstance().log(LogLevel::ERROR, "Elif statement condition must be a boolean expression");
				exit(1);
			}

			if (elifConditionValue.boolValue)
			{
				interpret(elif.second);
				return;
			}
		}

		interpret(node->elseBody);
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
			else if (literalNode->valueType == "float")
			{
				return Value(literalNode->floatValue);
			}
            else if (literalNode->valueType == "string")
            {
                return Value(literalNode->stringValue);
            }
			else if (literalNode->valueType == "bool")
			{
				return Value(literalNode->boolValue);
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
            return  ScopeManager::getInstance().getVariable(varNode->varName);
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
				else if (leftValue.valueType == "float" && rightValue.valueType == "float")
                {
                    return Value(leftValue.floatValue + rightValue.floatValue);
                }
                else if (leftValue.valueType == "int" && rightValue.valueType == "float")
                {
                    return Value(leftValue.intValue + rightValue.floatValue);
                }
                else if (leftValue.valueType == "float" && rightValue.valueType == "int")
                {
                    return Value(leftValue.floatValue + rightValue.intValue);
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
				else if (leftValue.valueType == "string" && rightValue.valueType == "bool")
				{
					return Value(leftValue.stringValue + (rightValue.boolValue ? "true" : "false"));
				}
				else if (leftValue.valueType == "bool" && rightValue.valueType == "string")
				{
					return Value((leftValue.boolValue ? "true" : "false") + rightValue.stringValue);
				}
				else if (leftValue.valueType == "string" && rightValue.valueType == "float")
				{
					return Value(leftValue.stringValue + std::to_string(rightValue.floatValue));
				}
				else if (leftValue.valueType == "float" && rightValue.valueType == "string")
				{
					return Value(std::to_string(leftValue.floatValue) + rightValue.stringValue);
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
				else if (leftValue.valueType == "float" && rightValue.valueType == "float")
				{
					return Value(leftValue.floatValue - rightValue.floatValue);
				}
				else if (leftValue.valueType == "int" && rightValue.valueType == "float")
				{
					return Value(leftValue.intValue - rightValue.floatValue);
				}
				else if (leftValue.valueType == "float" && rightValue.valueType == "int")
				{
					return Value(leftValue.floatValue - rightValue.intValue);
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
				else if (leftValue.valueType == "float" && rightValue.valueType == "float")
				{
					return Value(leftValue.floatValue * rightValue.floatValue);
				}
				else if (leftValue.valueType == "int" && rightValue.valueType == "float")
				{
					return Value(leftValue.intValue * rightValue.floatValue);
				}
				else if (leftValue.valueType == "float" && rightValue.valueType == "int")
				{
					return Value(leftValue.floatValue * rightValue.intValue);
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
				if ((rightValue.valueType == "int" && rightValue.intValue == 0) ||
					(rightValue.valueType == "float" && rightValue.floatValue == 0.0))
				{
					Logger::getInstance().log(LogLevel::ERROR, "Division by zero");
					exit(1);
				}
				if (leftValue.valueType == "int" && rightValue.valueType == "int")
				{
					return Value(leftValue.intValue / rightValue.intValue);
				}
				else if (leftValue.valueType == "float" && rightValue.valueType == "float")
				{
					return Value(leftValue.floatValue / rightValue.floatValue);
				}
				else if (leftValue.valueType == "int" && rightValue.valueType == "float")
				{
					return Value(leftValue.intValue / rightValue.floatValue);
				}
				else if (leftValue.valueType == "float" && rightValue.valueType == "int")
				{
					return Value(leftValue.floatValue / rightValue.intValue);
				}
				else
				{
					Logger::getInstance().log(LogLevel::ERROR, "Type mismatch in '/' operation");
					exit(1);
				}

				Logger::getInstance().log(LogLevel::ERROR, "Type mismatch in '/' operation");
				exit(1);
			}
			else if (exprNode->op == '&') // for &&
			{
				if (leftValue.valueType == "bool" && rightValue.valueType == "bool")
				{
					return Value(leftValue.boolValue && rightValue.boolValue);
				}
				else
				{
					Logger::getInstance().log(LogLevel::ERROR, "Type mismatch in '&&' operation");
					exit(1);
				}
			}
			else if (exprNode->op == '|') // for ||
			{
				if (leftValue.valueType == "bool" && rightValue.valueType == "bool")
				{
					return Value(leftValue.boolValue || rightValue.boolValue);
				}
				else
				{
					Logger::getInstance().log(LogLevel::ERROR, "Type mismatch in '||' operation");
					exit(1);
				}
			}
			else if (exprNode->op == TOKEN_TYPE_EQUALITY) // for ==
			{
				return Value(leftValue.intValue == rightValue.intValue);
			}
			else if (exprNode->op == TOKEN_TYPE_INEQUALITY) // for !=
			{
				return Value(leftValue.intValue != rightValue.intValue);
			}
			else if (exprNode->op == TOKEN_TYPE_LESS_EQUAL) // for <=
			{
				return Value(leftValue.intValue <= rightValue.intValue);
			}
			else if (exprNode->op == TOKEN_TYPE_GREATER_EQUAL) // for >=
			{
				return Value(leftValue.intValue >= rightValue.intValue);
			}
			else if (exprNode->op == TOKEN_TYPE_LESS) // for <
			{
				return Value(leftValue.intValue < rightValue.intValue);
			}
			else if (exprNode->op == TOKEN_TYPE_GREATER) // for >
			{
				return Value(leftValue.intValue > rightValue.intValue);
			}

            else
            {
                Logger::getInstance().log(LogLevel::ERROR, "Unsupported operator: " + std::string(1, exprNode->op));
                exit(1);
            }
        }
        case t_ast_node_type::AST_NODE_TYPE_CALL:
		{
			return interpretFunctionCall((t_ast_node_call *)node);
		}
		case t_ast_node_type::AST_NODE_TYPE_CAST: {
            t_ast_node_cast *castNode = static_cast<t_ast_node_cast *>(node);
            Value value = evaluateExpression(castNode->expr);

			if (castNode->castType == value.valueType) {
				return value;
			}
			else if (castNode->castType == "float" && value.valueType == "int") {
                return Value(static_cast<float>(value.intValue));
            }
            else if (castNode->castType == "int" && value.valueType == "float") {
                return Value(static_cast<int>(value.floatValue));
            }
            Logger::getInstance().log(LogLevel::ERROR, "Invalid cast operation");
            exit(1);
        }
		default:
			std::cout << (int)node->type << std::endl;
            Logger::getInstance().log(LogLevel::ERROR, "Unsupported AST node type in expression");
            exit(1);
    }
}



void Interpreter::interpretAssignStatement(t_ast_node_assign *node)
{
	if (node->expr == nullptr)
	{
		Value value = Value();
		value.valueType = node->varType;
		 ScopeManager::getInstance().setVariable(node->varName, value);
		return;
	}

    Value value = evaluateExpression(node->expr);

	if (node->varType == "auto")
		node->varType = value.valueType;
	if (node->varType == "")
	{
		std::string varType =  ScopeManager::getInstance().getVariableType(node->varName);
		if (varType != value.valueType)
		{
			Logger::getInstance().log(LogLevel::ERROR, "Type mismatch in assignment to variable '" + node->varName + "'");
			exit(1);
		}

		node->varType = varType;
	}

    if (node->varType == value.valueType)
    {
         ScopeManager::getInstance().setVariable(node->varName, value);
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
	if (value.valueType == "undefined")
	{
		Logger::getInstance().log(LogLevel::ERROR, "Cannot write an undefined value");
		exit(1);
	}
	if (node->writeType == 0)
		std::cout << value.toString();
	else
		std::cout << value.toString() << std::endl;
}
