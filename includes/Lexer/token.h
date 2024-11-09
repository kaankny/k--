#ifndef TOKEN_H
#define TOKEN_H

#include <string>

typedef enum e_token_type
{
	TOKEN_TYPE_EOF,
	TOKEN_TYPE_PLUS,
	TOKEN_TYPE_MINUS,
	TOKEN_TYPE_STAR,
	TOKEN_TYPE_SLASH,
	TOKEN_TYPE_LPAREN,
	TOKEN_TYPE_RPAREN,
	TOKEN_TYPE_SEMICOLON,
	TOKEN_TYPE_EQUAL,

	TOKEN_TYPE_INTLIT,
	TOKEN_TYPE_CHARLIT,
	TOKEN_TYPE_STRINGLIT,
	TOKEN_TYPE_IDENTIFIER,

	// 11
	TOKEN_TYPE_ASSIGN,
	TOKEN_TYPE_WRITE,
	TOKEN_TYPE_READ,
	
	TOKEN_TYPE_INT,
	TOKEN_TYPE_CHAR,
	TOKEN_TYPE_STRING
}   t_token_type;

typedef struct s_token
{
    t_token_type type;
    int intValue;
	char charValue;
	std::string identifier;
	std::string stringValue;
}   t_token;

#endif