#include "../../../includes/Parser/Parser.hpp"
#include "../../../includes/Logger.hpp"

t_ast_node *Parser::parseWriteStatement()
{
	int writeType = m_currentToken->type == TOKEN_TYPE_WRITE ? 0 : 1;
	advanceToken();

	if (m_currentToken->type != TOKEN_TYPE_LPAREN)
	{
		Logger::getInstance().log(LogLevel::ERROR, "Expected left parenthesis in write statement");
		exit(1);
	}

	advanceToken();

	t_ast_node *exprNode = parseComparison();

	if (m_currentToken->type != TOKEN_TYPE_RPAREN)
	{
		Logger::getInstance().log(LogLevel::ERROR, "Expected right parenthesis in write statement");
		exit(1);
	}

	advanceToken();

	t_ast_node_write *writeNode = new t_ast_node_write;
	writeNode->type = t_ast_node_type::AST_NODE_TYPE_WRITE;
	writeNode->expr = exprNode;
	writeNode->writeType = writeType;

	return writeNode;
}
