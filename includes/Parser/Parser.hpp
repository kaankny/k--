#pragma once

#include "ast.h"

class Parser
{
	public:
		static Parser& getInstance()
		{
			static Parser instance;
			return instance;
		}

		t_ast_node *createASTNode(int op, t_ast_node_type type, int intValue, t_ast_node *left, t_ast_node *right);
		t_ast_node *createASTLeaf(int op, int intValue);
		t_ast_node *createASTUnaryNode(int op, );

	private:
		Parser() {}
		Parser(Parser const&) = delete;
		void operator=(Parser const&) = delete;


};