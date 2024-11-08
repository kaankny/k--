#include "../includes/Lexer/Lexer.hpp"
#include "../includes/Parser/Parser.hpp"
#include "../includes/Logger.hpp"
#include "../includes/messages.h"

#include <sstream>

int main(int ac, char **av)
{
	if (ac < 2)
	{
		Logger::getInstance().log(LogLevel::ERROR, MSG_NO_INPUT_FILE);
		return (1);
	}

	Lexer::getInstance().init(av[1]);

	// while (Lexer::getInstance().getNextToken())
	// {
	// 	t_token token = Lexer::getInstance().getToken();
	// 	#ifdef DEBUGMODE
	// 		std::stringstream ss;
	// 		ss << "Token type: " << token.type;
	// 		if (token.type == TOKEN_TYPE_INTLIT)
	// 		{
	// 			ss << ", Token value: " << token.intValue;
	// 		}
	// 		Logger::getInstance().log(LogLevel::DEBUG, ss.str());
	// 	#endif

	// 	if (token.type == TOKEN_TYPE_EOF)
	// 	{
	// 		break;
	// 	}
	// }

	Lexer::getInstance().getNextToken();
	t_ast_node *node = Parser::getInstance().binexpr();
	int result = Parser::getInstance().interpretAST(node);
	#ifdef DEBUGMODE
		Logger::getInstance().log(LogLevel::DEBUG, "Result: " + std::to_string(result));
	#endif
    return (0);
}