#ifndef AST_H
#define AST_H

#include <string>

typedef enum class e_ast_node_type
{
	AST_NODE_TYPE_EXPR,
	AST_NODE_TYPE_LITERAL,
	AST_NODE_TYPE_VARIABLE,
	AST_NODE_TYPE_ASSIGN,
	AST_NODE_TYPE_WRITE,
	AST_NODE_TYPE_READ,
	AST_NODE_TYPE_IF,
	AST_NODE_TYPE_WHILE,
	AST_NODE_TYPE_FOR,
	AST_NODE_TYPE_BREAK,

	AST_NODE_TYPE_FUNCTION,
	AST_NODE_TYPE_CALL,
	AST_NODE_TYPE_RETURN,
}   t_ast_node_type;

typedef struct s_ast_node
{
	t_ast_node_type type;
	virtual ~s_ast_node() {}
}   t_ast_node;

typedef struct s_ast_node_expr : public s_ast_node
{
	t_ast_node *left;
	t_ast_node *right;
	char op;
	std::string exprType; // "int", "string"
}   t_ast_node_expr;

typedef struct s_ast_node_literal : public s_ast_node
{
	int intValue;
	std::string stringValue;
	std::string valueType; // "int", "string"	
	bool boolValue;
}   t_ast_node_literal;

typedef struct s_ast_node_variable : public s_ast_node
{
	std::string varName;
	std::string varType; // "int", "char", "string"
}   t_ast_node_variable;

typedef struct s_ast_node_assign : public s_ast_node
{
	std::string varType; // "int", "char", "string"
	std::string varName;

	t_ast_node *expr;
}   t_ast_node_assign;

typedef struct s_ast_node_write : public s_ast_node
{
	t_ast_node *expr;
	int	writeType; // 0: write, 1: writeln
}   t_ast_node_write;

typedef struct s_ast_node_if : public s_ast_node
{
	t_ast_node *condition;
	std::vector<t_ast_node *> ifBody;
	std::vector<std::pair<t_ast_node *, std::vector<t_ast_node *>>> elifBodies;
	std::vector<t_ast_node *> elseBody;
}   t_ast_node_if;

typedef struct s_ast_node_while : public s_ast_node
{
	t_ast_node *condition;
	std::vector<t_ast_node *> whileBody;
}	t_ast_node_while;

typedef struct s_ast_node_for : public s_ast_node
{
	t_ast_node *init;
	t_ast_node *condition;
	t_ast_node *update;
	std::vector<t_ast_node *> forBody;
}	t_ast_node_for;

typedef struct s_ast_node_read : public s_ast_node
{
	std::string varName;
}	t_ast_node_read;

typedef struct s_ast_node_break : public s_ast_node
{
}	t_ast_node_break;

typedef struct s_ast_node_function : public s_ast_node
{
	std::string functionName;
	std::string returnType;
	std::vector<std::pair<std::string, std::string>> parameters;
	std::vector<t_ast_node *> functionBody;
}	t_ast_node_function;

typedef struct s_ast_node_call : public s_ast_node
{
	std::string functionName;
	std::vector<t_ast_node *> arguments;
}	t_ast_node_call;

typedef struct s_ast_node_return : public s_ast_node
{
	t_ast_node *returnValue;
}	t_ast_node_return;

#endif