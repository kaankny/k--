#include "../includes/Parser/Parser.hpp"

t_ast_node *Parser::createASTNode(int op, t_ast_node_type type, int intValue, t_ast_node *left, t_ast_node *right)
{
	t_ast_node *node = new t_ast_node;
	node->op = op;
	node->type = type;
	node->intValue = intValue;
	node->left = left;
	node->right = right;
	return node;
}