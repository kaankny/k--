#include "../includes/Parser/Parser.hpp"
#include "../includes/Lexer/Lexer.hpp"
#include "../includes/Logger.hpp"
#include "../includes/messages.h"

t_ast_node_type Parser::getASTNodeType(t_token_type tokenType)
{
	switch (tokenType)
	{
		case TOKEN_TYPE_PLUS:
			return (AST_NODE_TYPE_ADD);
		case TOKEN_TYPE_MINUS:
			return (AST_NODE_TYPE_SUB);
		case TOKEN_TYPE_STAR:
			return (AST_NODE_TYPE_MUL);
		case TOKEN_TYPE_SLASH:
			return (AST_NODE_TYPE_DIV);
		case TOKEN_TYPE_INTLIT:
			return (AST_NODE_TYPE_INTLIT);
		default:
			Logger::getInstance().log(LogLevel::ERROR, MSG_INVALID_AST_NODE_TYPE(tokenType));
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
	return (node);
}

t_ast_node *Parser::createASTLeaf(int op, int intValue)
{
	return createASTNode(op, intValue, nullptr, nullptr);
}

t_ast_node *Parser::createASTUnaryNode(int op, t_ast_node *left, int intValue)
{
	return createASTNode(op, intValue, left, nullptr);
}

t_ast_node *Parser::primary(void)
{
	t_ast_node *node;
	t_token *token = Lexer::getInstance().getToken();

	switch (token->type)
	{
		case TOKEN_TYPE_INTLIT:
			node = createASTLeaf(AST_NODE_TYPE_INTLIT, token->intValue);
			Lexer::getInstance().getNextToken();
			return (node);
		case TOKEN_TYPE_EOF:
			return (nullptr);
		default:
			Logger::getInstance().log(LogLevel::ERROR, MSG_INVALID_AST_NODE_TYPE(token->type));
			exit(1);
	}
}

// Yeni fonksiyonlar: Öncelik seviyelerine göre işlem fonksiyonları
t_ast_node *Parser::multiplicative(void)
{
	t_ast_node *left = primary();
	t_token *token = Lexer::getInstance().getToken();

	while (token->type == TOKEN_TYPE_STAR || token->type == TOKEN_TYPE_SLASH)
	{
		t_ast_node_type nodeType = getASTNodeType(token->type);
		Lexer::getInstance().getNextToken();
		t_ast_node *right = primary();
		left = createASTNode(nodeType, 0, left, right);
		token = Lexer::getInstance().getToken();
	}
	return left;
}

t_ast_node *Parser::additive(void)
{
	t_ast_node *left = multiplicative();
	t_token *token = Lexer::getInstance().getToken();

	while (token->type == TOKEN_TYPE_PLUS || token->type == TOKEN_TYPE_MINUS)
	{
		t_ast_node_type nodeType = getASTNodeType(token->type);
		Lexer::getInstance().getNextToken();
		t_ast_node *right = multiplicative();
		left = createASTNode(nodeType, 0, left, right);
		token = Lexer::getInstance().getToken();
	}
	return left;
}

// Binexpr fonksiyonu en düşük öncelik seviyesinden başlar
t_ast_node *Parser::binexpr(void)
{
	return additive(); // Artık en düşük öncelikten başlayarak tüm işlemleri zincirleme olarak çağıracak
}

int Parser::interpretAST(t_ast_node *node)
{
	if (node == nullptr)
	{
		Logger::getInstance().log(LogLevel::ERROR, "Empty AST node.");
		return (0);
	}
	else if (node->op == AST_NODE_TYPE_INTLIT)
	{
		return (node->intValue);
	}
	else
	{
		int leftValue = 0;
		int rightValue = 0;
		if (node->left)
			leftValue = interpretAST(node->left);
		if (node->right)
			rightValue = interpretAST(node->right);

		switch (node->op)
		{
			case AST_NODE_TYPE_ADD:
				return (leftValue + rightValue);
			case AST_NODE_TYPE_SUB:
				return (leftValue - rightValue);
			case AST_NODE_TYPE_MUL:
				return (leftValue * rightValue);
			case AST_NODE_TYPE_DIV:
				return (leftValue / rightValue);
			case AST_NODE_TYPE_INTLIT:
				return (node->intValue);
			default:
				Logger::getInstance().log(LogLevel::ERROR, MSG_INVALID_AST_NODE_TYPE(node->op));
				exit(1);
		}
	}
}
