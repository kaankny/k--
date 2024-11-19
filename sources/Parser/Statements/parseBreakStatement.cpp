#include "../../../includes/Parser/Parser.hpp"

t_ast_node *Parser::parseBreakStatement()
{
	advanceToken(); // `break` anahtar kelimesini geç

	t_ast_node_break *breakNode = new t_ast_node_break;
	breakNode->type = t_ast_node_type::AST_NODE_TYPE_BREAK;

	return breakNode;
}