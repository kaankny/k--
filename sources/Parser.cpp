#include "../includes/Parser/Parser.hpp"
#include "../includes/Lexer/Lexer.hpp"
#include "../includes/Logger.hpp"
#include "../includes/messages.h"

std::vector<t_ast_node *> Parser::parse()
{
	Lexer::getInstance().getNextToken();
	this->m_currentToken = Lexer::getInstance().getToken();
	std::vector<t_ast_node *> ast;
	while (m_currentToken->type != TOKEN_TYPE_EOF)
	{
		t_ast_node *node = parseStatement();
		if (node)
			ast.push_back(node);
		if (Lexer::getInstance().getNextToken())
		{
			m_currentToken = Lexer::getInstance().getToken();
		}
		else
		{
			Logger::getInstance().log(LogLevel::ERROR, MSG_TOKEN_NOT_FOUND);
			exit(1);
		}
	}

	return (ast);
}

t_ast_node *Parser::parseStatement()
{
    switch (m_currentToken->type)
    {
        case TOKEN_TYPE_ASSIGN:
            return parseAssignStatement();
        case TOKEN_TYPE_WRITE:
        case TOKEN_TYPE_WRITELN:
            return parseWriteStatement();
        case TOKEN_TYPE_IDENTIFIER: // Değişken adı ile başlıyorsa atama olarak yorumlanır
            return parseVariableAssign();
        default:
            std::cout << "Invalid statement type: " << m_currentToken->type << std::endl;
            Logger::getInstance().log(LogLevel::ERROR, MSG_INVALID_STATEMENT(m_currentToken->type));
            exit(1);
    }
    return nullptr;
}

t_ast_node *Parser::parseVariableAssign()
{
    // Değişken ismini al
    std::string varName = m_currentToken->identifier;
    advanceToken();

    // Eşittir sembolünü kontrol et
    if (m_currentToken->type == TOKEN_TYPE_EQUAL)
    {
        advanceToken();
    }
    else
    {
        Logger::getInstance().log(LogLevel::ERROR, "Expected '=' in assignment");
        exit(1);
    }

    // İfadeyi çözümle
    t_ast_node *exprNode = parseExpression();

    // Atama AST düğümünü oluştur
    t_ast_node_assign *assignNode = new t_ast_node_assign;
    assignNode->type = t_ast_node_type::AST_NODE_TYPE_ASSIGN;
    assignNode->varName = varName;
    assignNode->expr = exprNode;

    return assignNode;
}



t_ast_node *Parser::parseExpression()
{
    t_ast_node *node = parseTerm();

    while (m_currentToken->type == TOKEN_TYPE_PLUS || m_currentToken->type == TOKEN_TYPE_MINUS)
    {
        char op = (m_currentToken->type == TOKEN_TYPE_PLUS) ? '+' : '-';
        advanceToken();
        t_ast_node *rightNode = parseTerm();

        t_ast_node_expr *newNode = new t_ast_node_expr;
        newNode->type = t_ast_node_type::AST_NODE_TYPE_EXPR;
        newNode->left = node;
        newNode->op = op;
        newNode->right = rightNode;

        node = newNode;
    }

    return node;
}



t_ast_node *Parser::parseTerm()
{
	t_ast_node *node = parseFactor();

	while (m_currentToken->type == TOKEN_TYPE_STAR || m_currentToken->type == TOKEN_TYPE_SLASH)
	{
		char op = (m_currentToken->type == TOKEN_TYPE_STAR) ? '*' : '/';
		advanceToken();
		t_ast_node_expr *newNode = new t_ast_node_expr;
		newNode->type = t_ast_node_type::AST_NODE_TYPE_EXPR;
		newNode->left = node;
		newNode->op = op;
		newNode->right = parseFactor();
		node = newNode;
	}

	return node;
}

t_ast_node *Parser::parseFactor()
{
    if (m_currentToken->type == TOKEN_TYPE_INTLIT)
    {
        t_ast_node_literal *node = new t_ast_node_literal;
        node->type = t_ast_node_type::AST_NODE_TYPE_LITERAL;
        node->intValue = m_currentToken->intValue;
        node->valueType = "int";
        advanceToken();
        return node;
    }
    else if (m_currentToken->type == TOKEN_TYPE_STRINGLIT)
    {
        t_ast_node_literal *node = new t_ast_node_literal;
        node->type = t_ast_node_type::AST_NODE_TYPE_LITERAL;
        node->stringValue = m_currentToken->stringValue;
        node->valueType = "string";
        advanceToken();
        return node;
    }
    else if (m_currentToken->type == TOKEN_TYPE_IDENTIFIER)
    {
        t_ast_node_variable *node = new t_ast_node_variable;
        node->type = t_ast_node_type::AST_NODE_TYPE_VARIABLE;
        node->varName = m_currentToken->identifier;
        advanceToken();
        return node;
    }
	else if (m_currentToken->type == TOKEN_TYPE_TRUE || m_currentToken->type == TOKEN_TYPE_FALSE)
	{
		t_ast_node_literal *node = new t_ast_node_literal;
		node->type = t_ast_node_type::AST_NODE_TYPE_LITERAL;
		node->valueType = "bool";
		node->boolValue = m_currentToken->type == TOKEN_TYPE_TRUE;
		advanceToken();
		return node;
	}
    else if (m_currentToken->type == TOKEN_TYPE_LPAREN)
    {
        advanceToken();
        t_ast_node *node = parseExpression();
        if (m_currentToken->type != TOKEN_TYPE_RPAREN)
        {
            Logger::getInstance().log(LogLevel::ERROR, MSG_EXPECTED_RPAREN);
            exit(1);
        }
        advanceToken();
        return node;
    }
    else
    {
        Logger::getInstance().log(LogLevel::ERROR, MSG_INVALID_TOKEN_TYPE(m_currentToken->type));
        exit(1);
    }

    return nullptr;
}


t_ast_node *Parser::parseAssignStatement()
{
    advanceToken();

    // Değişken tipini al
    std::string varType;
    if (m_currentToken->type == TOKEN_TYPE_INT)
    {
        varType = "int";
    }
    else if (m_currentToken->type == TOKEN_TYPE_STRING)
    {
        varType = "string";
    }
	else if (m_currentToken->type == TOKEN_TYPE_BOOL)
	{
		varType = "bool";
	}
	else if (m_currentToken->type == TOKEN_TYPE_AUTO)
	{
		varType = "auto";
	}
    else
    {
        Logger::getInstance().log(LogLevel::ERROR, "Invalid variable type in assignment");
        exit(1);
    }

    advanceToken();

    // Değişken ismini al
    if (m_currentToken->type != TOKEN_TYPE_IDENTIFIER)
    {
        Logger::getInstance().log(LogLevel::ERROR, "Expected variable name in assignment");
        exit(1);
    }

    std::string varName = m_currentToken->identifier;
    advanceToken();

    t_ast_node *exprNode = nullptr; // Varsayılan olarak null bırakılır

    // Eşittir sembolü kontrolü, değer varsa işlenir
    if (m_currentToken->type == TOKEN_TYPE_EQUAL)
    {
        advanceToken();
        exprNode = parseExpression(); // İfade varsa çözümlenir
    }

    // Atama AST düğümünü oluştur
    t_ast_node_assign *assignNode = new t_ast_node_assign;
    assignNode->type = t_ast_node_type::AST_NODE_TYPE_ASSIGN;
    assignNode->varType = varType;
    assignNode->varName = varName;
    assignNode->expr = exprNode; // İfade varsa atanır, yoksa null kalır

    return assignNode;
}





t_ast_node *Parser::parseWriteStatement()
{
	int writeType = m_currentToken->type == TOKEN_TYPE_WRITE ? 0 : 1;
	advanceToken();

	if (m_currentToken->type != TOKEN_TYPE_LPAREN)
	{
		Logger::getInstance().log(LogLevel::ERROR, "Expected left parenthesis in write statement");
		exit(1);
	}

	advanceToken();

	t_ast_node *exprNode = parseExpression();

	if (m_currentToken->type != TOKEN_TYPE_RPAREN)
	{
		Logger::getInstance().log(LogLevel::ERROR, "Expected right parenthesis in write statement");
		exit(1);
	}

	advanceToken();

	t_ast_node_write *writeNode = new t_ast_node_write;
	writeNode->type = t_ast_node_type::AST_NODE_TYPE_WRITE;
	writeNode->expr = exprNode;
	writeNode->writeType = writeType;

	return writeNode;
}

void Parser::advanceToken()
{
	if (Lexer::getInstance().getNextToken())
	{
		m_currentToken = Lexer::getInstance().getToken();
	}
	else
	{
		Logger::getInstance().log(LogLevel::ERROR, MSG_TOKEN_NOT_FOUND);
		exit(1);
	}
}