#ifndef AST_H
#define AST_H

#include <string>

typedef enum class e_ast_node_type
{
	AST_NODE_TYPE_ASSIGN,
	AST_NODE_TYPE_WRITE
}   t_ast_node_type;

typedef struct s_ast_node
{
	t_ast_node_type type;
	virtual ~s_ast_node() {}
}   t_ast_node;

typedef struct s_ast_node_assign : public s_ast_node
{
	std::string varType; // "int", "char", "string"
	std::string varName;
	int intValue;
}   t_ast_node_assign;

typedef struct s_ast_node_write : public s_ast_node
{
	std::string varName;
}   t_ast_node_write;

#endif