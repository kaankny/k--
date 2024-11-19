#include "../../../includes/Parser/Parser.hpp"
#include "../../../includes/Logger.hpp"

t_ast_node *Parser::parseForStatement()
{
    advanceToken();  // `for` anahtar kelimesini geçin

    if (m_currentToken->type != TOKEN_TYPE_LPAREN)
    {
        Logger::getInstance().log(LogLevel::ERROR, "Expected '(' after 'for'");
        exit(1);
    }
    advanceToken();

    // Başlangıç ifadesini ayrıştırın (örneğin, `int i = 0` veya `;` ise boş geçin)
    t_ast_node *init = nullptr;
    if (m_currentToken->type != TOKEN_TYPE_SEMICOLON)
    {
		if (m_currentToken->type == TOKEN_TYPE_ASSIGN)
			init = parseAssignStatement();
		else
			init = parseVariableAssign();
    }
    
    if (m_currentToken->type != TOKEN_TYPE_SEMICOLON)
    {
        Logger::getInstance().log(LogLevel::ERROR, "Expected ';' after initialization in 'for' statement");
        exit(1);
    }
    advanceToken();

    // Koşul ifadesini ayrıştırın (örneğin, `i < 10` veya boş bırakılabilir)
    t_ast_node *condition = nullptr;
    if (m_currentToken->type != TOKEN_TYPE_SEMICOLON)
    {
        condition = parseComparison();
    }

    if (m_currentToken->type != TOKEN_TYPE_SEMICOLON)
    {
        Logger::getInstance().log(LogLevel::ERROR, "Expected ';' after condition in 'for' statement");
        exit(1);
    }
    advanceToken();

    // Artış/azalış ifadesini ayrıştırın (örneğin, `i++` veya boş bırakılabilir)
    t_ast_node *increment = nullptr;
    if (m_currentToken->type != TOKEN_TYPE_RPAREN)
    {
        increment = parseVariableAssign();
    }

    if (m_currentToken->type != TOKEN_TYPE_RPAREN)
    {
        Logger::getInstance().log(LogLevel::ERROR, "Expected ')' after increment in 'for' statement");
        exit(1);
    }
    advanceToken();

    // Döngü gövdesini ayrıştırın
    std::vector<t_ast_node *> forBody = parseBlock();

	if (m_currentToken->type != TOKEN_TYPE_ENDFOR)
	{
		Logger::getInstance().log(LogLevel::ERROR, "Expected 'endfor;' after for statement");
		exit(1);
	}

	advanceToken(); // Move past `endfor`

    // `for` düğümünü oluşturun ve bileşenleri ekleyin
    t_ast_node_for *forNode = new t_ast_node_for;
    forNode->type = t_ast_node_type::AST_NODE_TYPE_FOR;
    forNode->init = init;
    forNode->condition = condition;
    forNode->update = increment;
    forNode->forBody = forBody;

    return forNode;
}

