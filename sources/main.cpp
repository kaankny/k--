#include "../includes/Lexer/Lexer.hpp"
#include "../includes/Parser/Parser.hpp"
#include "../includes/Logger.hpp"
#include "../includes/messages.h"
#include "../includes/Interpreter.hpp"

#include <sstream>

int main(int ac, char **av)
{
	if (ac < 2)
	{
		return (1);
	}

	Lexer::getInstance().init(av[1]);
	std::vector<t_ast_node *> ast_nodes = Parser::getInstance().parse();
	Interpreter::getInstance().interpret(ast_nodes);

	// t_token *token = Lexer::getInstance().getToken();
	// while (Lexer::getInstance().getNextToken())
	// {
	// 	#ifdef DEBUGMODE
	// 		std::stringstream ss;
	// 		ss << "Token type: " << token->type;
	// 		if (token->type == TOKEN_TYPE_INTLIT)
	// 			ss << ", int value: " << token->intValue;
	// 		else if (token->type == TOKEN_TYPE_IDENTIFIER)
	// 			ss << ", identifier: " << token->identifier;
	// 		else if (token->type == TOKEN_TYPE_STRINGLIT)
	// 			ss << ", string value: " << token->stringValue;
	// 		else if (token->type == TOKEN_TYPE_CHARLIT)
	// 			ss << ", char value: " << token->charValue;
	// 		else if (token->type == TOKEN_TYPE_INT)
	// 			ss << ", int keyword";
	// 		else if (token->type == TOKEN_TYPE_EOF)
	// 			ss << ", EOF";
	// 		else if (token->type == TOKEN_TYPE_PLUS)
	// 			ss << ", +";
	// 		else if (token->type == TOKEN_TYPE_MINUS)
	// 			ss << ", -";
	// 		else if (token->type == TOKEN_TYPE_STAR)
	// 			ss << ", *";
	// 		else if (token->type == TOKEN_TYPE_SLASH)
	// 			ss << ", /";
	// 		else if (token->type == TOKEN_TYPE_LPAREN)
	// 			ss << ", (";
	// 		else if (token->type == TOKEN_TYPE_RPAREN)
	// 			ss << ", )";
	// 		else if (token->type == TOKEN_TYPE_SEMICOLON)
	// 			ss << ", ;";
	// 		else if (token->type == TOKEN_TYPE_ASSIGN)
	// 			ss << ", assign";
	// 		else if (token->type == TOKEN_TYPE_WRITE)
	// 			ss << ", write";
	// 		else if (token->type == TOKEN_TYPE_READ)
	// 			ss << ", read";
			
	// 		Logger::getInstance().log(LogLevel::DEBUG, ss.str());
	// 	#endif

	// 	if (token->type == TOKEN_TYPE_EOF)
	// 	{
	// 		break;
	// 	}
	// }

	// Lexer::getInstance().getNextToken();
	// t_ast_node *node = Parser::getInstance().binexpr();
	// int result = Parser::getInstance().interpretAST(node);
	// #ifdef DEBUGMODE
	// 	Logger::getInstance().log(LogLevel::DEBUG, "Result: " + std::to_string(result));
	// #endif
    return (0);
}