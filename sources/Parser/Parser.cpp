#include "../../includes/Parser/Parser.hpp"
#include "../../includes/Lexer/Lexer.hpp"
#include "../../includes/Logger.hpp"
#include "../../includes/messages.h"
#include "../../includes/Types.hpp"

std::vector<t_ast_node *> Parser::parse()
{
	Lexer::getInstance().getNextToken();
	this->m_currentToken = Lexer::getInstance().getToken();
	std::vector<t_ast_node *> ast;
	while (m_currentToken->type != TOKEN_TYPE_EOF)
	{
		t_ast_node *node = parseStatement();
		if (node)
			ast.push_back(node);
		if (Lexer::getInstance().getNextToken())
		{
			m_currentToken = Lexer::getInstance().getToken();
		}
		else
		{
			Logger::getInstance().log(LogLevel::ERROR, MSG_TOKEN_NOT_FOUND);
			exit(1);
		}
	}

	return (ast);
}

t_ast_node *Parser::parseStatement()
{
    switch (m_currentToken->type)
    {
        case TOKEN_TYPE_ASSIGN:
            return parseAssignStatement();
        case TOKEN_TYPE_WRITE:
        case TOKEN_TYPE_WRITELN:
            return parseWriteStatement();
        case TOKEN_TYPE_IDENTIFIER:
            return parseVariableAssign();
		case TOKEN_TYPE_IF:
			return parseIfStatement();
		case TOKEN_TYPE_WHILE:
			return parseWhileStatement();
		case TOKEN_TYPE_FOR:
			return parseForStatement();
		case TOKEN_TYPE_READ:
			return parseReadStatement();

		case TOKEN_TYPE_BREAK:
			return parseBreakStatement();
		case TOKEN_TYPE_CONTINUE:
			return parseContinueStatement();

		case TOKEN_TYPE_CALL:
			return parseFunctionCall();
		case TOKEN_TYPE_FUNCTION:
			return parseFunctionDefinition();
		case TOKEN_TYPE_RETURN:
			return parseReturnStatement();

		case TOKEN_TYPE_SWITCH:
			return parseSwitchStatement();
        default:
            std::cout << "Invalid statement type: " << m_currentToken->type << std::endl;
            Logger::getInstance().log(LogLevel::ERROR, MSG_INVALID_STATEMENT(m_currentToken->type));
            exit(1);
    }
    return nullptr;
}

std::vector<t_ast_node *> Parser::parseBlock()
{
    std::vector<t_ast_node *> blockStatements;

    // Expect `{` to start the block
    if (m_currentToken->type != TOKEN_TYPE_LBRACE)
    {
		std::cout << m_currentToken->type << std::endl;
        Logger::getInstance().log(LogLevel::ERROR, "Expected '{' to start block");
        exit(1);
    }
    advanceToken();

    // Parse statements until `}` is encountered
    while (m_currentToken->type != TOKEN_TYPE_RBRACE)
    {
		t_ast_node *stmt = parseStatement();
		if (stmt)
			blockStatements.push_back(stmt);
		if (Lexer::getInstance().getNextToken())
		{
			m_currentToken = Lexer::getInstance().getToken();
		}
		else
		{
			Logger::getInstance().log(LogLevel::ERROR, MSG_TOKEN_NOT_FOUND);
			exit(1);
		}
    }

    // Move past `}`
    advanceToken();

    return blockStatements;
}

t_ast_node *Parser::parseComparison()
{
    t_ast_node *node = parseExpression();

    while (m_currentToken->type == TOKEN_TYPE_EQUALITY || m_currentToken->type == TOKEN_TYPE_INEQUALITY ||
           m_currentToken->type == TOKEN_TYPE_LESS_EQUAL || m_currentToken->type == TOKEN_TYPE_GREATER_EQUAL ||
           m_currentToken->type == TOKEN_TYPE_LESS || m_currentToken->type == TOKEN_TYPE_GREATER)
    {
        int op = m_currentToken->type;
        advanceToken();
        t_ast_node *rightNode = parseExpression();

        t_ast_node_expr *newNode = new t_ast_node_expr;
        newNode->type = t_ast_node_type::AST_NODE_TYPE_EXPR;
        newNode->left = node;
        newNode->op = op;
        newNode->right = rightNode;

        node = newNode;
    }

    return node;
}

t_ast_node *Parser::parseExpression()
{
    t_ast_node *node = parseTerm();

    while (m_currentToken->type == TOKEN_TYPE_PLUS || m_currentToken->type == TOKEN_TYPE_MINUS ||
           m_currentToken->type == TOKEN_TYPE_AND || m_currentToken->type == TOKEN_TYPE_OR)
    {
        char op;
        if (m_currentToken->type == TOKEN_TYPE_PLUS)
            op = '+';
        else if (m_currentToken->type == TOKEN_TYPE_MINUS)
            op = '-';
        else if (m_currentToken->type == TOKEN_TYPE_AND)
            op = '&';
        else
            op = '|';

        advanceToken();
        t_ast_node *rightNode = parseTerm();

        t_ast_node_expr *newNode = new t_ast_node_expr;
        newNode->type = t_ast_node_type::AST_NODE_TYPE_EXPR;
        newNode->left = node;
        newNode->op = op;
        newNode->right = rightNode;

        node = newNode;
    }

    return node;
}

t_ast_node *Parser::parseTerm()
{
	t_ast_node *node = parseFactor();

	while (m_currentToken->type == TOKEN_TYPE_STAR || m_currentToken->type == TOKEN_TYPE_SLASH)
	{
		char op = (m_currentToken->type == TOKEN_TYPE_STAR) ? '*' : '/';
		advanceToken();
		t_ast_node_expr *newNode = new t_ast_node_expr;
		newNode->type = t_ast_node_type::AST_NODE_TYPE_EXPR;
		newNode->left = node;
		newNode->op = op;
		newNode->right = parseFactor();
		node = newNode;
	}

	return node;
}

t_ast_node *Parser::parseFactor()
{
	if (m_currentToken->type == TOKEN_TYPE_CAST_FLOAT || m_currentToken->type == TOKEN_TYPE_CAST_INT) {
        std::string castType = (m_currentToken->type == TOKEN_TYPE_CAST_FLOAT) ? "float" : "int";
        advanceToken(); // Parantez içindeki tür dönüşümünü geç

        if (m_currentToken->type != TOKEN_TYPE_RPAREN) {
            Logger::getInstance().log(LogLevel::ERROR, "Expected ')' after cast type");
            exit(1);
        }
        advanceToken();

        t_ast_node *exprNode = parseFactor();

        t_ast_node_cast *castNode = new t_ast_node_cast;
        castNode->type = t_ast_node_type::AST_NODE_TYPE_CAST;
        castNode->castType = castType;
        castNode->expr = exprNode;

        return castNode;
    }
    else if (m_currentToken->type == TOKEN_TYPE_INTLIT)
    {
        t_ast_node_literal *node = new t_ast_node_literal;
        node->type = t_ast_node_type::AST_NODE_TYPE_LITERAL;
        node->intValue = m_currentToken->intValue;
        node->valueType = "int";
        advanceToken();
        return node;
    }
    else if (m_currentToken->type == TOKEN_TYPE_FLOATLIT)
    {
        t_ast_node_literal *node = new t_ast_node_literal;
        node->type = t_ast_node_type::AST_NODE_TYPE_LITERAL;
        node->floatValue = m_currentToken->floatValue;
        node->valueType = "float";
        advanceToken();
        return node;
    }
    else if (m_currentToken->type == TOKEN_TYPE_STRINGLIT)
    {
        t_ast_node_literal *node = new t_ast_node_literal;
        node->type = t_ast_node_type::AST_NODE_TYPE_LITERAL;
        node->stringValue = m_currentToken->stringValue;
        node->valueType = "string";
        advanceToken();
        return node;
    }
	else if (m_currentToken->type == TOKEN_TYPE_CHARLIT)
	{
		t_ast_node_literal *node = new t_ast_node_literal;
		node->type = t_ast_node_type::AST_NODE_TYPE_LITERAL;
		node->charValue = m_currentToken->charValue;
		node->valueType = "char";
		advanceToken();
		return node;
	}
    else if (m_currentToken->type == TOKEN_TYPE_IDENTIFIER)
    {
        t_ast_node_variable *node = new t_ast_node_variable;
        node->type = t_ast_node_type::AST_NODE_TYPE_VARIABLE;
        node->varName = m_currentToken->identifier;
        advanceToken();
        return node;
    }
	else if (m_currentToken->type == TOKEN_TYPE_TRUE || m_currentToken->type == TOKEN_TYPE_FALSE)
	{
		t_ast_node_literal *node = new t_ast_node_literal;
		node->type = t_ast_node_type::AST_NODE_TYPE_LITERAL;
		node->valueType = "bool";
		node->boolValue = m_currentToken->type == TOKEN_TYPE_TRUE;
		advanceToken();
		return node;
	}
    else if (m_currentToken->type == TOKEN_TYPE_LPAREN)
    {
        advanceToken();
        t_ast_node *node = parseComparison();
        if (m_currentToken->type != TOKEN_TYPE_RPAREN)
        {
            Logger::getInstance().log(LogLevel::ERROR, MSG_EXPECTED_RPAREN);
            exit(1);
        }
        advanceToken();
        return node;
    }
	else if (m_currentToken->type == TOKEN_TYPE_CALL)
	{
		return parseFunctionCall();
	}
    else
    {
        Logger::getInstance().log(LogLevel::ERROR, MSG_INVALID_TOKEN_TYPE(m_currentToken->type));
        exit(1);
    }

    return nullptr;
}

void Parser::advanceToken()
{
	if (Lexer::getInstance().getNextToken())
	{
		m_currentToken = Lexer::getInstance().getToken();
	}
	else
	{
		Logger::getInstance().log(LogLevel::ERROR, MSG_TOKEN_NOT_FOUND);
		exit(1);
	}
}