#include "../../../includes/Parser/Parser.hpp"
#include "../../../includes/Logger.hpp"

t_ast_node *Parser::parseFunctionCall()
{
	// call add (1, 2)
	advanceToken(); // `call` anahtar kelimesini geç

	if (m_currentToken->type != TOKEN_TYPE_IDENTIFIER)
	{
		Logger::getInstance().log(LogLevel::ERROR, "Expected function name after 'call'");
		exit(1);
	}

	std::string functionName = m_currentToken->identifier;

	advanceToken();

	if (m_currentToken->type != TOKEN_TYPE_LPAREN)
	{
		Logger::getInstance().log(LogLevel::ERROR, "Expected '(' after function name in 'call' statement");
		exit(1);
	}

	advanceToken();

	std::vector<t_ast_node *> arguments;
	while (m_currentToken->type != TOKEN_TYPE_RPAREN)
	{
		t_ast_node *expr = parseExpression();
		arguments.push_back(expr);

		if (m_currentToken->type == TOKEN_TYPE_COMMA)
		{
			advanceToken();
		}
		else if (m_currentToken->type != TOKEN_TYPE_RPAREN)
		{
			Logger::getInstance().log(LogLevel::ERROR, "Expected ',' or ')' in function call arguments");
			exit(1);
		}
	}

	advanceToken();

	t_ast_node_call *callNode = new t_ast_node_call;
	callNode->type = t_ast_node_type::AST_NODE_TYPE_CALL;
	callNode->functionName = functionName;
	callNode->arguments = arguments;

	return callNode;
}

