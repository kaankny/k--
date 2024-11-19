#include "../../../includes/Parser/Parser.hpp"
#include "../../../includes/Logger.hpp"

t_ast_node *Parser::parseVariableAssign()
{
    // Değişken ismini al
    std::string varName = m_currentToken->identifier;
    advanceToken();

    // Eşittir sembolünü kontrol et
    if (m_currentToken->type == TOKEN_TYPE_EQUAL)
    {
        advanceToken();
    }
    else
    {
		std::cout << m_currentToken->type << std::endl;
        Logger::getInstance().log(LogLevel::ERROR, "Expected '=' in assignment");
        exit(1);
    }

    // İfadeyi çözümle
    t_ast_node *exprNode = parseComparison();

    // Atama AST düğümünü oluştur
    t_ast_node_assign *assignNode = new t_ast_node_assign;
    assignNode->type = t_ast_node_type::AST_NODE_TYPE_ASSIGN;
    assignNode->varName = varName;
    assignNode->expr = exprNode;

    return assignNode;
}