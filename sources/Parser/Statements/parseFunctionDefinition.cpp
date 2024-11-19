#include "../../../includes/Parser/Parser.hpp"
#include "../../../includes/Logger.hpp"
#include "../../../includes/Types.hpp"

t_ast_node *Parser::parseFunctionDefinition()
{
    advanceToken(); // `function` anahtar kelimesini geç

    // Fonksiyon adını al
    if (m_currentToken->type != TOKEN_TYPE_IDENTIFIER)
    {
        Logger::getInstance().log(LogLevel::ERROR, "Expected function name after 'function'");
        exit(1);
    }
    std::string functionName = m_currentToken->identifier;
    advanceToken();

    // Parametre listesini ayrıştır
    if (m_currentToken->type != TOKEN_TYPE_LPAREN)
    {
        Logger::getInstance().log(LogLevel::ERROR, "Expected '(' after function name");
        exit(1);
    }
    advanceToken();

    std::vector<std::pair<std::string, std::string>> parameters;
    while (m_currentToken->type == TOKEN_TYPE_IDENTIFIER)
    {
        std::string paramName = m_currentToken->identifier;
        advanceToken();

        if (m_currentToken->type != TOKEN_TYPE_COLON)
        {
            Logger::getInstance().log(LogLevel::ERROR, "Expected ':' after parameter name");
            exit(1);
        }
        advanceToken();

        if (m_currentToken->type != TOKEN_TYPE_INT && m_currentToken->type != TOKEN_TYPE_STRING && m_currentToken->type != TOKEN_TYPE_BOOL)
        {
            Logger::getInstance().log(LogLevel::ERROR, "Expected type after ':' in parameter");
            exit(1);
        }
		std::string paramType = TypeMapping::getTypeName(m_currentToken->type);
		if (paramType == "unknown")
		{
			Logger::getInstance().log(LogLevel::ERROR, "Unknown type in parameter");
			exit(1);
		}
        parameters.push_back({paramName, paramType});
        advanceToken();

        if (m_currentToken->type == TOKEN_TYPE_COMMA)
        {
            advanceToken();
        }
    }

    if (m_currentToken->type != TOKEN_TYPE_RPAREN)
    {
        Logger::getInstance().log(LogLevel::ERROR, "Expected ')' after parameter list");
        exit(1);
    }
    advanceToken();

    // Dönüş türünü ayrıştır
    std::string returnType = "void";  // Varsayılan dönüş türü
    if (m_currentToken->type == TOKEN_TYPE_ARROW)
    {
        advanceToken();
		returnType = TypeMapping::getTypeName(m_currentToken->type);
		if (returnType == "unknown")
		{
			Logger::getInstance().log(LogLevel::ERROR, "Unknown return type");
			exit(1);
		}
        advanceToken();
    }
    // Fonksiyon gövdesini ayrıştır
    std::vector<t_ast_node *> functionBody = parseBlock();

	if (m_currentToken->type != TOKEN_TYPE_ENDFUNCTION)
	{
		Logger::getInstance().log(LogLevel::ERROR, "Expected 'endfunction' after function body");
		exit(1);
	}

	advanceToken(); // Move past `endfunction`


    // Yeni fonksiyon düğümünü oluştur
    t_ast_node_function *functionNode = new t_ast_node_function;
    functionNode->type = t_ast_node_type::AST_NODE_TYPE_FUNCTION;
    functionNode->functionName = functionName;
    functionNode->parameters = parameters;
    functionNode->returnType = returnType;
    functionNode->functionBody = functionBody;

    return functionNode;
}
