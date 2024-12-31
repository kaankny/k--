#include "../../../includes/Parser/Parser.hpp"
#include "../../../includes/Logger.hpp"
#include "../../../includes/Types.hpp"

/// @brief Parses an assignment statement in the source code.
/// This function advances the token stream, retrieves the variable type and name,
/// and ensures they are valid. If the variable type is unknown or the variable name
/// is missing, it logs an error and exits the program.
/// @return A pointer to the AST node representing the assignment statement.
t_ast_node *Parser::parseAssignStatement()
{
    advanceToken();

    // Değişken tipini al
    std::string varType;
	varType = TypeMapping::getTypeName(m_currentToken->type);
	if (varType == "unknown")
	{
		Logger::getInstance().log(LogLevel::ERROR, "Unknown variable type in assignment");
		exit(1);
	}
    advanceToken();

    // Değişken ismini al
    if (m_currentToken->type != TOKEN_TYPE_IDENTIFIER)
    {
        Logger::getInstance().log(LogLevel::ERROR, "Expected variable name in assignment");
        exit(1);
    }

    std::string varName = m_currentToken->identifier;
    advanceToken();

    t_ast_node *exprNode = nullptr; // Varsayılan olarak null bırakılır

    // Eşittir sembolü kontrolü, değer varsa işlenir
    if (m_currentToken->type == TOKEN_TYPE_EQUAL)
    {
        advanceToken();
        exprNode = parseComparison(); // İfade varsa çözümlenir
    }


    // Atama AST düğümünü oluştur
    t_ast_node_assign *assignNode = new t_ast_node_assign;
    assignNode->type = t_ast_node_type::AST_NODE_TYPE_ASSIGN;
    assignNode->varType = varType;
    assignNode->varName = varName;
    assignNode->expr = exprNode; // İfade varsa atanır, yoksa null kalır

    return assignNode;
}