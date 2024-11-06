#ifndef AST_H
#define AST_H

typedef enum e_AST_NODE_TYPE
{
	AST_NODE_TYPE_ADD,
	AST_NODE_TYPE_SUB,
	AST_NODE_TYPE_MUL,
	AST_NODE_TYPE_DIV,
	AST_NODE_TYPE_INTLIT,
	AST_NODE_TYPE_EOF
}   t_ast_node_type;

typedef struct s_ast_node
{
	int op;
	t_ast_node_type type;
	int intValue;

	// OPERATOR
	struct s_ast_node *left;
	struct s_ast_node *right;

	// 
}   t_ast_node;

#endif