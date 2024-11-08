#pragma once

#include <vector>

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

		t_ast_node *parseAssignStatement();
		t_ast_node *parseWriteStatement();
		t_ast_node *parseStatement();

};