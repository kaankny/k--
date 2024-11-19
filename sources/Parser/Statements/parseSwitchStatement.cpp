#include "../../../includes/Parser/Parser.hpp"
#include "../../../includes/Logger.hpp"

t_ast_node *Parser::parseSwitchStatement()
{
	advanceToken(); // `switch` anahtar kelimesini geç

	if (m_currentToken->type != TOKEN_TYPE_LPAREN)
	{
		Logger::getInstance().log(LogLevel::ERROR, "Expected '(' after 'switch'");
		exit(1);
	}
	advanceToken();

	t_ast_node *switchExpr = parseExpression();

	if (m_currentToken->type != TOKEN_TYPE_RPAREN)
	{
		Logger::getInstance().log(LogLevel::ERROR, "Expected ')' after switch expression");
		exit(1);
	}
	advanceToken();

	if (m_currentToken->type != TOKEN_TYPE_LBRACE)
	{
		Logger::getInstance().log(LogLevel::ERROR, "Expected '{' after switch expression");
		exit(1);
	}
	advanceToken();

	std::vector<std::pair<t_ast_node *, std::vector<t_ast_node *>>> cases;
	std::vector<t_ast_node *> defaultCase;
	while (m_currentToken->type == TOKEN_TYPE_CASE || m_currentToken->type == TOKEN_TYPE_DEFAULT)
    {
        if (m_currentToken->type == TOKEN_TYPE_CASE)
        {
            advanceToken();

            t_ast_node *caseExpr = parseExpression();

            if (m_currentToken->type != TOKEN_TYPE_COLON)
            {
                Logger::getInstance().log(LogLevel::ERROR, "Expected ':' after case expression");
                exit(1);
            }
            advanceToken();

            std::vector<t_ast_node *> caseBody = parseBlock();
            cases.push_back({caseExpr, caseBody});
        }
        else if (m_currentToken->type == TOKEN_TYPE_DEFAULT)
        {
            advanceToken();

            if (m_currentToken->type != TOKEN_TYPE_COLON)
            {
                Logger::getInstance().log(LogLevel::ERROR, "Expected ':' after 'default'");
                exit(1);
            }
            advanceToken();

            defaultCase = parseBlock();
        }
    }

    if (m_currentToken->type != TOKEN_TYPE_RBRACE)
    {
        Logger::getInstance().log(LogLevel::ERROR, "Expected '}' to close switch body");
        exit(1);
    }
    advanceToken();

	if (m_currentToken->type != TOKEN_TYPE_ENDSWITCH)
	{
		Logger::getInstance().log(LogLevel::ERROR, "Expected 'endswitch' after switch body");
		exit(1);
	}
	advanceToken();

    t_ast_node_switch *switchNode = new t_ast_node_switch;
    switchNode->type = t_ast_node_type::AST_NODE_TYPE_SWITCH;
    switchNode->switchExpr = switchExpr;
    switchNode->cases = cases;
    switchNode->defaultCase = defaultCase;

    return switchNode;

}