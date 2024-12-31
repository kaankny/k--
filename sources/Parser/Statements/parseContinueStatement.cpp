#include "../../../includes/Parser/Parser.hpp"

/// @brief Parses a continue statement in the source code.
/// This function advances the token stream past the `continue` keyword and creates
/// an AST node representing the continue statement.
/// @return A pointer to the AST node representing the continue statement.
t_ast_node *Parser::parseContinueStatement()
{
    advanceToken(); // `continue` anahtar kelimesini geç

    t_ast_node_continue *continueNode = new t_ast_node_continue;
    continueNode->type = t_ast_node_type::AST_NODE_TYPE_CONTINUE;

    return continueNode;
}