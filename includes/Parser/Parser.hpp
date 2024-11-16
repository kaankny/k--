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
		t_ast_node *parseReadStatement();
		t_ast_node *parseStatement();
		t_ast_node *parseVariableAssign();
		t_ast_node *parseComparison();


		t_ast_node *parseIfStatement();
		t_ast_node *parseWhileStatement();
		t_ast_node *parseForStatement();
		std::vector<t_ast_node *> parseBlock();

		t_ast_node *parseBreakStatement();
		t_ast_node *parseContinueStatement();

		t_ast_node *parseFunctionDefinition();
		t_ast_node *parseFunctionCall();
		t_ast_node *parseReturnStatement();

		t_ast_node *parseLogicalAnd();
		t_ast_node *parseLogicalOr();

		t_ast_node *parseAdditive();
		t_ast_node *parseMultiplicative();
};