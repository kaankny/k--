#include "../../../includes/Parser/Parser.hpp"

/// @brief Parses a break statement in the source code.
/// This function advances the token stream past the `break` keyword and creates
/// an AST node representing the break statement.
/// @return A pointer to the AST node representing the break statement.
t_ast_node *Parser::parseBreakStatement()
{
    advanceToken(); // `break` anahtar kelimesini geç

    t_ast_node_break *breakNode = new t_ast_node_break;
    breakNode->type = t_ast_node_type::AST_NODE_TYPE_BREAK;

    return breakNode;
}