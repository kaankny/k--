#pragma once

#include "ast.h"
#include "token.h"

class Parser
{
	public:
		static Parser& getInstance()
		{
			static Parser instance;
			return instance;
		}

		t_ast_node_type getASTNodeType(t_token_type tokenType);

		t_ast_node *createASTNode(int op, int intValue, t_ast_node *left, t_ast_node *right);
		t_ast_node *createASTLeaf(int op, int intValue);
		t_ast_node *createASTUnaryNode(int op, t_ast_node *left, int intValue);

	private:
		Parser() {}
		Parser(Parser const&) = delete;
		void operator=(Parser const&) = delete;


};