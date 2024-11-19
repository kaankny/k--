#include "../../../includes/Parser/Parser.hpp"


t_ast_node *Parser::parseReturnStatement()
{
	advanceToken(); // `return` anahtar kelimesini geç

	t_ast_node *expr = parseExpression();

	t_ast_node_return *returnNode = new t_ast_node_return;
	returnNode->type = t_ast_node_type::AST_NODE_TYPE_RETURN;
	returnNode->returnValue = expr;

	return returnNode;
}