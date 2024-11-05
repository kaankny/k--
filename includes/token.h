#ifndef TOKEN_H
#define TOKEN_H

typedef enum e_token_type
{
	TOKEN_TYPE_PLUS,
	TOKEN_TYPE_MINUS,
	TOKEN_TYPE_STAR,
	TOKEN_TYPE_SLASH,
	TOKEN_TYPE_INTLIT,
	TOKEN_TYPE_LPAREN,
	TOKEN_TYPE_RPAREN
}   t_token_type;

typedef struct s_token
{
    t_token_type type;
    int intValue;
}   t_token;

#endif