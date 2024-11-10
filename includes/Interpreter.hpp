#pragma once

#include <vector>
#include <unordered_map>

#include "./Parser/ast.h"
#include "./Value.hpp"
#include "./Scope.hpp"

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

		Scope context;

		void interpretAssignStatement(t_ast_node_assign *node);
		void interpretWriteStatement(t_ast_node_write *node);
		void interpretIfStatement(t_ast_node_if *node);
		void interpretWhileStatement(t_ast_node_while *node);

		Value evaluateExpression(t_ast_node *node);
		std::string evaluateStringExpression(t_ast_node *node);
};