#include "../../../includes/Parser/Parser.hpp"
#include "../../../includes/Logger.hpp"

t_ast_node *Parser::parseIfStatement()
{
    // Parse the initial `if` condition
    advanceToken(); // Assume current token is "if"
    if (m_currentToken->type != TOKEN_TYPE_LPAREN)
    {
        Logger::getInstance().log(LogLevel::ERROR, "Expected '(' after 'if'");
        exit(1);
    }
    advanceToken();

    // Parse the condition expression
    t_ast_node *condition = parseComparison();

    if (m_currentToken->type != TOKEN_TYPE_RPAREN)
    {
        Logger::getInstance().log(LogLevel::ERROR, "Expected ')' after condition in 'if' statement");
        exit(1);
    }
    advanceToken();

    // Parse the `if` body within `{}` braces
    std::vector<t_ast_node *> ifBody = parseBlock();

    // Parse `elif` blocks if present
    std::vector<std::pair<t_ast_node *, std::vector<t_ast_node *>>> elifBodies;
    while (m_currentToken->type == TOKEN_TYPE_ELSE_IF)
    {
        advanceToken();
        if (m_currentToken->type != TOKEN_TYPE_LPAREN)
        {
            Logger::getInstance().log(LogLevel::ERROR, "Expected '(' after 'elif'");
            exit(1);
        }
        advanceToken();

        t_ast_node *elifCondition = parseComparison();
        if (m_currentToken->type != TOKEN_TYPE_RPAREN)
        {
            Logger::getInstance().log(LogLevel::ERROR, "Expected ')' after condition in 'elif' statement");
            exit(1);
        }
        advanceToken();

        std::vector<t_ast_node *> elifBody = parseBlock();
        elifBodies.push_back(std::make_pair(elifCondition, elifBody));
    }

    // Parse the `else` block if present
    std::vector<t_ast_node *> elseBody;
    if (m_currentToken->type == TOKEN_TYPE_ELSE)
    {
        advanceToken();
        elseBody = parseBlock();
    }

    if (m_currentToken->type != TOKEN_TYPE_ENDIF)
	{
		Logger::getInstance().log(LogLevel::ERROR, "Expected 'endif;' after if-else statement");
		exit(1);
	}
	advanceToken(); // Move past `endif`

    // Create and return the if-elif-else node
    t_ast_node_if *ifNode = new t_ast_node_if;
    ifNode->type = t_ast_node_type::AST_NODE_TYPE_IF;
    ifNode->condition = condition;
    ifNode->ifBody = ifBody;
    ifNode->elifBodies = elifBodies;
    ifNode->elseBody = elseBody;

    return ifNode;
}
