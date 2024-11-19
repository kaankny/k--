#include "../../../includes/Parser/Parser.hpp"

t_ast_node *Parser::parseContinueStatement()
{
	advanceToken(); // `continue` anahtar kelimesini geç

	t_ast_node_continue *continueNode = new t_ast_node_continue;
	continueNode->type = t_ast_node_type::AST_NODE_TYPE_CONTINUE;

	return continueNode;
}
