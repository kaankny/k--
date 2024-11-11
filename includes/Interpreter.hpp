#pragma once

#include <vector>
#include <unordered_map>

#include "./Parser/ast.h"
#include "./Value.hpp"
#include "./ScopeManager.hpp"

class Interpreter
{
	public:
		static Interpreter& getInstance()
		{
			static Interpreter instance;
			return instance;
		}

		void interpret(std::vector<t_ast_node *> ast);

	private:
		Interpreter() {}
		Interpreter(Interpreter const&) = delete;
		void operator=(Interpreter const&) = delete;

		std::unordered_map<std::string, t_ast_node_function *> functions;

		void interpretAssignStatement(t_ast_node_assign *node);
		void interpretWriteStatement(t_ast_node_write *node);
		void interpretReadStatement(t_ast_node_read *node);
		void interpretIfStatement(t_ast_node_if *node);
		void interpretWhileStatement(t_ast_node_while *node);
		void interpretForStatement(t_ast_node_for *node);
		void interpretFunctionDefinition(t_ast_node_function *node);
		Value interpretFunctionCall(t_ast_node_call *node);

		Value evaluateExpression(t_ast_node *node);
		std::string evaluateStringExpression(t_ast_node *node);
};