#include "../includes/Parser/Parser.hpp"
#include "../includes/Logger.hpp"

t_ast_node_type Parser::getASTNodeType(t_token_type tokenType)
{
	switch (tokenType)
	{
		case TOKEN_TYPE_PLUS:
			return AST_NODE_TYPE_ADD;
		case TOKEN_TYPE_MINUS:
			return AST_NODE_TYPE_SUB;
		case TOKEN_TYPE_STAR:
			return AST_NODE_TYPE_MUL;
		case TOKEN_TYPE_SLASH:
			return AST_NODE_TYPE_DIV;
		case TOKEN_TYPE_INTLIT:
			return AST_NODE_TYPE_INTLIT;
		default:
			Logger::getInstance().log(LogLevel::ERROR, "Invalid token type.");
			exit(1);
	}
}

t_ast_node *Parser::createASTNode(int op, int intValue, t_ast_node *left, t_ast_node *right)
{
	t_ast_node *node = new t_ast_node;
	node->op = op;
	node->intValue = intValue;
	node->left = left;
	node->right = right;
	return node;
}

t_ast_node *Parser::createASTLeaf(int op, int intValue)
{
	return createASTNode(op, intValue, nullptr, nullptr);
}

t_ast_node *Parser::createASTUnaryNode(int op, t_ast_node *left, int intValue)
{
	return createASTNode(op, intValue, left, nullptr);
}