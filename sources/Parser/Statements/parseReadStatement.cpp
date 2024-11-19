#include "../../../includes/Parser/Parser.hpp"
#include "../../../includes/Logger.hpp"

t_ast_node *Parser::parseReadStatement()
{
    advanceToken(); // `read` anahtar kelimesini geç

    if (m_currentToken->type != TOKEN_TYPE_LPAREN)
    {
        Logger::getInstance().log(LogLevel::ERROR, "Expected '(' after 'read'");
        exit(1);
    }
    advanceToken();

    // Değişken adını al
    if (m_currentToken->type != TOKEN_TYPE_IDENTIFIER)
    {
        Logger::getInstance().log(LogLevel::ERROR, "Expected variable name in 'read' statement");
        exit(1);
    }

    std::string varName = m_currentToken->identifier;
    advanceToken();

    if (m_currentToken->type != TOKEN_TYPE_RPAREN)
    {
        Logger::getInstance().log(LogLevel::ERROR, "Expected ')' after variable name in 'read' statement");
        exit(1);
    }
    advanceToken();

    // `read` AST düğümünü oluştur
    t_ast_node_read *readNode = new t_ast_node_read;
    readNode->type = t_ast_node_type::AST_NODE_TYPE_READ;
    readNode->varName = varName;

    return readNode;
}