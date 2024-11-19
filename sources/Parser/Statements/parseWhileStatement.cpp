#include "../../../includes/Parser/Parser.hpp"
#include "../../../includes/Logger.hpp"

t_ast_node *Parser::parseWhileStatement()
{
	advanceToken(); // Assume current token is "while"
	if (m_currentToken->type != TOKEN_TYPE_LPAREN)
	{
		Logger::getInstance().log(LogLevel::ERROR, "Expected '(' after 'while'");
		exit(1);
	}
	advanceToken();

	t_ast_node *condition = parseComparison();

	if (m_currentToken->type != TOKEN_TYPE_RPAREN)
	{
		Logger::getInstance().log(LogLevel::ERROR, "Expected ')' after condition in 'while' statement");
		exit(1);
	}
	advanceToken();

	std::vector<t_ast_node *> whileBody = parseBlock();

	if (m_currentToken->type != TOKEN_TYPE_ENDWHILE)
	{
		Logger::getInstance().log(LogLevel::ERROR, "Expected 'endwhile;' after while statement");
		exit(1);
	}
	advanceToken(); // Move past `endwhile`

	t_ast_node_while *whileNode = new t_ast_node_while;
	whileNode->type = t_ast_node_type::AST_NODE_TYPE_WHILE;
	whileNode->condition = condition;
	whileNode->whileBody = whileBody;

	return whileNode;
}