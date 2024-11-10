#pragma once

#include <vector>
#include <unordered_map>

#include "ast.h"
#include "../Lexer/token.h"


class Parser
{
	public:
		static Parser& getInstance()
		{
			static Parser instance;
			return instance;
		}

		std::vector<t_ast_node *> parse();

	private:
		Parser() {}
		Parser(Parser const&) = delete;
		void operator=(Parser const&) = delete;

		t_token *m_currentToken;

		void advanceToken();

		t_ast_node *parseExpression();
		t_ast_node *parseTerm();
		t_ast_node *parseFactor();
		t_ast_node *parseAssignStatement();
		t_ast_node *parseWriteStatement();
		t_ast_node *parseStatement();
		t_ast_node *parseVariableAssign();
		t_ast_node *parseComparison();

		t_ast_node *parseIfStatement();
		std::vector<t_ast_node *> parseBlock();

		t_ast_node *parseLogicalAnd();
		t_ast_node *parseLogicalOr();

		t_ast_node *parseAdditive();
		t_ast_node *parseMultiplicative();
};