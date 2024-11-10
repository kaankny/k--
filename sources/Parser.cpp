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
		case TOKEN_TYPE_IF:
			return parseIfStatement();
		case TOKEN_TYPE_WHILE:
			return parseWhileStatement();
        default:
            std::cout << "Invalid statement type: " << m_currentToken->type << std::endl;
            Logger::getInstance().log(LogLevel::ERROR, MSG_INVALID_STATEMENT(m_currentToken->type));
            exit(1);
    }
    return nullptr;
}

t_ast_node *Parser::parseWhileStatement()
{
	advanceToken(); // Assume current token is "while"
	if (m_currentToken->type != TOKEN_TYPE_LPAREN)
	{
		Logger::getInstance().log(LogLevel::ERROR, "Expected '(' after 'while'");
		exit(1);
	}
	advanceToken();

	t_ast_node *condition = parseComparison();

	if (m_currentToken->type != TOKEN_TYPE_RPAREN)
	{
		Logger::getInstance().log(LogLevel::ERROR, "Expected ')' after condition in 'while' statement");
		exit(1);
	}
	advanceToken();

	std::vector<t_ast_node *> whileBody = parseBlock();

	if (m_currentToken->type != TOKEN_TYPE_ENDWHILE)
	{
		Logger::getInstance().log(LogLevel::ERROR, "Expected 'endwhile;' after while statement");
		exit(1);
	}
	advanceToken(); // Move past `endwhile`

	t_ast_node_while *whileNode = new t_ast_node_while;
	whileNode->type = t_ast_node_type::AST_NODE_TYPE_WHILE;
	whileNode->condition = condition;
	whileNode->whileBody = whileBody;

	return whileNode;
}

t_ast_node *Parser::parseIfStatement()
{
    // Parse the initial `if` condition
    advanceToken(); // Assume current token is "if"
    if (m_currentToken->type != TOKEN_TYPE_LPAREN)
    {
        Logger::getInstance().log(LogLevel::ERROR, "Expected '(' after 'if'");
        exit(1);
    }
    advanceToken();

    // Parse the condition expression
    t_ast_node *condition = parseComparison();

    if (m_currentToken->type != TOKEN_TYPE_RPAREN)
    {
        Logger::getInstance().log(LogLevel::ERROR, "Expected ')' after condition in 'if' statement");
        exit(1);
    }
    advanceToken();

    // Parse the `if` body within `{}` braces
    std::vector<t_ast_node *> ifBody = parseBlock();

    // Parse `elif` blocks if present
    std::vector<std::pair<t_ast_node *, std::vector<t_ast_node *>>> elifBodies;
    while (m_currentToken->type == TOKEN_TYPE_ELSE_IF)
    {
        advanceToken();
        if (m_currentToken->type != TOKEN_TYPE_LPAREN)
        {
            Logger::getInstance().log(LogLevel::ERROR, "Expected '(' after 'elif'");
            exit(1);
        }
        advanceToken();

        t_ast_node *elifCondition = parseComparison();
        if (m_currentToken->type != TOKEN_TYPE_RPAREN)
        {
            Logger::getInstance().log(LogLevel::ERROR, "Expected ')' after condition in 'elif' statement");
            exit(1);
        }
        advanceToken();

        std::vector<t_ast_node *> elifBody = parseBlock();
        elifBodies.push_back(std::make_pair(elifCondition, elifBody));
    }

    // Parse the `else` block if present
    std::vector<t_ast_node *> elseBody;
    if (m_currentToken->type == TOKEN_TYPE_ELSE)
    {
        advanceToken();
        elseBody = parseBlock();
    }

    if (m_currentToken->type != TOKEN_TYPE_ENDIF)
	{
		Logger::getInstance().log(LogLevel::ERROR, "Expected 'endif;' after if-else statement");
		exit(1);
	}
	advanceToken(); // Move past `endif`

    // Create and return the if-elif-else node
    t_ast_node_if *ifNode = new t_ast_node_if;
    ifNode->type = t_ast_node_type::AST_NODE_TYPE_IF;
    ifNode->condition = condition;
    ifNode->ifBody = ifBody;
    ifNode->elifBodies = elifBodies;
    ifNode->elseBody = elseBody;

    return ifNode;
}

std::vector<t_ast_node *> Parser::parseBlock()
{
    std::vector<t_ast_node *> blockStatements;

    // Expect `{` to start the block
    if (m_currentToken->type != TOKEN_TYPE_LBRACE)
    {
        Logger::getInstance().log(LogLevel::ERROR, "Expected '{' to start block");
        exit(1);
    }
    advanceToken();

    // Parse statements until `}` is encountered
    while (m_currentToken->type != TOKEN_TYPE_RBRACE)
    {
		t_ast_node *stmt = parseStatement();
		if (stmt)
			blockStatements.push_back(stmt);
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

    // Move past `}`
    advanceToken();

    return blockStatements;
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
    t_ast_node *exprNode = parseComparison();

    // Atama AST düğümünü oluştur
    t_ast_node_assign *assignNode = new t_ast_node_assign;
    assignNode->type = t_ast_node_type::AST_NODE_TYPE_ASSIGN;
    assignNode->varName = varName;
    assignNode->expr = exprNode;

    return assignNode;
}

t_ast_node *Parser::parseComparison()
{
    t_ast_node *node = parseExpression();

    while (m_currentToken->type == TOKEN_TYPE_EQUALITY || m_currentToken->type == TOKEN_TYPE_INEQUALITY ||
           m_currentToken->type == TOKEN_TYPE_LESS_EQUAL || m_currentToken->type == TOKEN_TYPE_GREATER_EQUAL ||
           m_currentToken->type == TOKEN_TYPE_LESS || m_currentToken->type == TOKEN_TYPE_GREATER)
    {
        int op = m_currentToken->type;
        advanceToken();
        t_ast_node *rightNode = parseExpression();

        t_ast_node_expr *newNode = new t_ast_node_expr;
        newNode->type = t_ast_node_type::AST_NODE_TYPE_EXPR;
        newNode->left = node;
        newNode->op = op;
        newNode->right = rightNode;

        node = newNode;
    }

    return node;
}

t_ast_node *Parser::parseExpression()
{
    t_ast_node *node = parseTerm();

    while (m_currentToken->type == TOKEN_TYPE_PLUS || m_currentToken->type == TOKEN_TYPE_MINUS ||
           m_currentToken->type == TOKEN_TYPE_AND || m_currentToken->type == TOKEN_TYPE_OR)
    {
        char op;
        if (m_currentToken->type == TOKEN_TYPE_PLUS)
            op = '+';
        else if (m_currentToken->type == TOKEN_TYPE_MINUS)
            op = '-';
        else if (m_currentToken->type == TOKEN_TYPE_AND)
            op = '&';
        else
            op = '|';

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
        t_ast_node *node = parseComparison();
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
        exprNode = parseComparison(); // İfade varsa çözümlenir
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

	t_ast_node *exprNode = parseComparison();

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