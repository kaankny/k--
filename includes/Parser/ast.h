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
	AST_NODE_TYPE_IF,
	AST_NODE_TYPE_WHILE
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

#endif