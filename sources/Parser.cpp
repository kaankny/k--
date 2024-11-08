#include "../includes/Parser/Parser.hpp"
#include "../includes/Lexer/Lexer.hpp"
#include "../includes/Logger.hpp"
#include "../includes/messages.h"

std::vector<t_ast_node *> Parser::parse()
{
	this->m_currentToken = Lexer::getInstance().getToken();
	std::vector<t_ast_node *> ast;

	while (Lexer::getInstance().getNextToken())
	{
		if (m_currentToken->type == TOKEN_TYPE_EOF)
			break;

		t_ast_node *node = parseStatement();
		if (node)
			ast.push_back(node);
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
			return parseWriteStatement();
		default:
			Logger::getInstance().log(LogLevel::ERROR, MSG_INVALID_STATEMENT(m_currentToken->type));
			exit(1);
	}
	return nullptr;
}

t_ast_node *Parser::parseAssignStatement()
{
	// ASSIGN INT A 5;
	advanceToken();

	std::string varType;
	if (m_currentToken->type == TOKEN_TYPE_INT)
		varType = "int";
	else
	{
		Logger::getInstance().log(LogLevel::ERROR, MSG_INVALID_AST_NODE_TYPE(m_currentToken->type));
		exit(1);
	}

	advanceToken();

	if (m_currentToken->type != TOKEN_TYPE_IDENTIFIER)
	{
		Logger::getInstance().log(LogLevel::ERROR, MSG_INVALID_AST_NODE_TYPE(m_currentToken->type));
		exit(1);
	}

	std::string varName = m_currentToken->identifier;
	advanceToken();

	if (m_currentToken->type != TOKEN_TYPE_INTLIT)
	{
		Logger::getInstance().log(LogLevel::ERROR, MSG_INVALID_AST_NODE_TYPE(m_currentToken->type));
		exit(1);
	}

	int intValue = m_currentToken->intValue;

	advanceToken();

	if (m_currentToken->type != TOKEN_TYPE_SEMICOLON)
	{
		Logger::getInstance().log(LogLevel::ERROR, MSG_INVALID_AST_NODE_TYPE(m_currentToken->type));
		exit(1);
	}

	advanceToken();

	t_ast_node_assign *node = new t_ast_node_assign;
	node->type = t_ast_node_type::AST_NODE_TYPE_ASSIGN;
	node->varType = varType;
	node->varName = varName;
	node->intValue = intValue;

	return node;
}

t_ast_node *Parser::parseWriteStatement()
{
	return nullptr;
}

void Parser::advanceToken()
{
	if (Lexer::getInstance().getNextToken())
		m_currentToken = Lexer::getInstance().getToken();
	else
	{
		Logger::getInstance().log(LogLevel::ERROR, MSG_TOKEN_NOT_FOUND);
		exit(1);
	}
}