#ifndef TOKEN_H
#define TOKEN_H

#include <string>

typedef enum e_token_type
{
	TOKEN_TYPE_EOF, // End of file
	TOKEN_TYPE_PLUS, // +
	TOKEN_TYPE_MINUS, // -
	TOKEN_TYPE_STAR, // *
	TOKEN_TYPE_SLASH, // /
	TOKEN_TYPE_PERCENT, // %

	TOKEN_TYPE_COLON, // :
	TOKEN_TYPE_COMMA, // ,
	TOKEN_TYPE_LPAREN, // (
	TOKEN_TYPE_RPAREN, // )
	TOKEN_TYPE_LBRACE, // {
	TOKEN_TYPE_RBRACE, // }
	TOKEN_TYPE_SEMICOLON, // ;
	TOKEN_TYPE_EQUAL, // =
	TOKEN_TYPE_EXCLAMATION, // !

	TOKEN_TYPE_EQUALITY, // ==
	TOKEN_TYPE_INEQUALITY, // !=
	TOKEN_TYPE_LESS, // <
	TOKEN_TYPE_LESS_EQUAL, // <=
	TOKEN_TYPE_GREATER, // >
	TOKEN_TYPE_GREATER_EQUAL, // >=

	TOKEN_TYPE_AND, // &&
	TOKEN_TYPE_OR, // ||

	TOKEN_TYPE_INTLIT, // Integer literal
	TOKEN_TYPE_FLOATLIT, // Float literal
	TOKEN_TYPE_DOUBLELIT, // Double literal
	TOKEN_TYPE_CHARLIT, // Character literal
	TOKEN_TYPE_STRINGLIT, // String literal
	TOKEN_TYPE_IDENTIFIER, // Identifier

	TOKEN_TYPE_TRUE, // true
	TOKEN_TYPE_FALSE, // false

	TOKEN_TYPE_ASSIGN, // assign
	TOKEN_TYPE_WRITE, // write
	TOKEN_TYPE_WRITELN, // writeln
	TOKEN_TYPE_READ, // read

	TOKEN_TYPE_BREAK, // break
	TOKEN_TYPE_CONTINUE, // continue

	TOKEN_TYPE_SWITCH, // switch
	TOKEN_TYPE_CASE, // case
	TOKEN_TYPE_DEFAULT, // default
	TOKEN_TYPE_ENDSWITCH, // endswitch

	TOKEN_TYPE_IF, // if
	TOKEN_TYPE_ELSE_IF, // elseif
	TOKEN_TYPE_ELSE, // else
	TOKEN_TYPE_ENDIF, // endif

	TOKEN_TYPE_WHILE, // while
	TOKEN_TYPE_ENDWHILE, // endwhile
	TOKEN_TYPE_FOR, // for
	TOKEN_TYPE_ENDFOR, // endfor

	TOKEN_TYPE_FUNCTION, // function
	TOKEN_TYPE_ENDFUNCTION, // endfunction
	TOKEN_TYPE_CALL, // call
	TOKEN_TYPE_RETURN, // return
	TOKEN_TYPE_ARROW, // ->
	
	TOKEN_TYPE_AUTO, // auto
	TOKEN_TYPE_INT, // int
	TOKEN_TYPE_FLOAT, // float
	TOKEN_TYPE_DOUBLE, // double
	TOKEN_TYPE_CHAR, // char
	TOKEN_TYPE_STRING, // string
	TOKEN_TYPE_BOOL, // bool

	TOKEN_TYPE_CAST_FLOAT, // (float)
	TOKEN_TYPE_CAST_INT, // (int)
}   t_token_type;

typedef struct s_token
{
    t_token_type type;

    int intValue;
	float floatValue;
	char charValue;
	std::string identifier;
	std::string stringValue;
}   t_token;

#endif