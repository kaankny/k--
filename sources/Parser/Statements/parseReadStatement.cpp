#include "../../../includes/Parser/Parser.hpp"
#include "../../../includes/Logger.hpp"

t_ast_node *Parser::parseReadStatement()
{
	advanceToken(); // `read` anahtar kelimesini geç

	if (m_currentToken->type != TOKEN_TYPE_LPAREN)
	{
		Logger::getInstance().log(LogLevel::ERROR, "Expected '(' after 'read'");
		exit(1);
	}
	advanceToken();

	t_ast_node_read *readNode = new t_ast_node_read;
	readNode->type = t_ast_node_type::AST_NODE_TYPE_READ;

	// İlk token ya FD (int) olabilir ya da doğrudan varName
	if (m_currentToken->type == TOKEN_TYPE_INTLIT)
	{
		readNode->fd = m_currentToken->intValue;
		advanceToken();

		if (m_currentToken->type != TOKEN_TYPE_COMMA)
		{
			Logger::getInstance().log(LogLevel::ERROR, "Expected ',' after fd in 'read' statement");
			exit(1);
		}
		advanceToken();
	}
	else
	{
		readNode->fd = 0; // stdin varsay
	}

	// varName
	if (m_currentToken->type != TOKEN_TYPE_IDENTIFIER)
	{
		Logger::getInstance().log(LogLevel::ERROR, "Expected variable name in 'read' statement");
		exit(1);
	}
	readNode->varName = m_currentToken->identifier;
	advanceToken();

	if (m_currentToken->type != TOKEN_TYPE_COMMA)
	{
		Logger::getInstance().log(LogLevel::ERROR, "Expected ',' after variable name in 'read' statement");
		exit(1);
	}
	advanceToken();

	// varType
	if (m_currentToken->type != TOKEN_TYPE_INT &&
        m_currentToken->type != TOKEN_TYPE_FLOAT &&
        m_currentToken->type != TOKEN_TYPE_DOUBLE &&
        m_currentToken->type != TOKEN_TYPE_CHAR &&
        m_currentToken->type != TOKEN_TYPE_STRING &&
        m_currentToken->type != TOKEN_TYPE_BOOL)
	{
        std::cout << m_currentToken->type << std::endl;
		Logger::getInstance().log(LogLevel::ERROR, "Expected variable type in 'read' statement");
		exit(1);
	}
	readNode->varType = m_currentToken->type;
	advanceToken();

	// Opsiyonel: virgül varsa mesaj da olabilir
	if (m_currentToken->type == TOKEN_TYPE_COMMA)
	{
		advanceToken();

		if (m_currentToken->type != TOKEN_TYPE_STRINGLIT)
		{
			Logger::getInstance().log(LogLevel::ERROR, "Expected message string after ',' in 'read' statement");
			exit(1);
		}
		readNode->message = m_currentToken->stringValue;
		advanceToken();
	}
	else
	{
		// Mesaj verilmemişse boş bırak
		readNode->message = "";
	}

	if (m_currentToken->type != TOKEN_TYPE_RPAREN)
	{
		Logger::getInstance().log(LogLevel::ERROR, "Expected ')' at end of 'read' statement");
		exit(1);
	}
	advanceToken();

	return readNode;
}
