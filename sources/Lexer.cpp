#include "../includes/Lexer/Lexer.hpp"
#include "../includes/Logger.hpp"
#include "../includes/messages.h"

#include <fstream>

void Lexer::init(const char *inputFilePath)
{
	this->m_inputFileName = inputFilePath;
	std::ifstream file(inputFilePath);
	if (!file.is_open())
	{
		Logger::getInstance().log(LogLevel::ERROR, MSG_COULD_NOT_OPEN_FILE(inputFilePath));
		exit(1);
	}

	std::string line;
	while (std::getline(file, line))
	{
		this->m_inputFileContent += line;
		this->m_inputFileContent += '\n';
	}
}


bool Lexer::getNextToken()
{
	this->skipWhitespaces();
	this->skipComments();

	if (this->m_currentIndex >= this->m_inputFileContent.size())
	{
		this->m_token.type = TOKEN_TYPE_EOF;
		return true;
	}

	char currentChar = this->m_inputFileContent[this->m_currentIndex];

	if (currentChar == '+')
	{
		this->m_token.type = TOKEN_TYPE_PLUS;
	}
	else if (currentChar == '-')
	{
		this->m_token.type = TOKEN_TYPE_MINUS;
	}
	else if (currentChar == '*')
	{
		this->m_token.type = TOKEN_TYPE_STAR;
	}
	else if (currentChar == '/')
	{
		this->m_token.type = TOKEN_TYPE_SLASH;
	}
	else if (currentChar == '(')
	{
		this->m_token.type = TOKEN_TYPE_LPAREN;
	}
	else if (currentChar == ')')
	{
		this->m_token.type = TOKEN_TYPE_RPAREN;
	}
	else if (currentChar >= '0' && currentChar <= '9')
	{
		this->m_token.type = TOKEN_TYPE_INTLIT;
		this->m_token.intValue = 0;
		while (this->m_currentIndex < this->m_inputFileContent.size() && this->m_inputFileContent[this->m_currentIndex] >= '0' && this->m_inputFileContent[this->m_currentIndex] <= '9')
		{
			this->m_token.intValue = this->m_token.intValue * 10 + this->m_inputFileContent[this->m_currentIndex] - '0';
			this->m_currentIndex++;
		}
		this->m_currentIndex--;
	}
	else
	{
		exit(1);
	}
	this->m_currentIndex++;
	return true;
}

void Lexer::skipWhitespaces()
{
	while (this->m_currentIndex < this->m_inputFileContent.size() && (this->m_inputFileContent[this->m_currentIndex] == ' ' || this->m_inputFileContent[this->m_currentIndex] == '\t' || this->m_inputFileContent[this->m_currentIndex] == '\n'))
	{
		if (this->m_inputFileContent[this->m_currentIndex] == '\n')
		{
			this->m_currentLine++;
			this->m_currentColumn = 0;
		}
		else
		{
			this->m_currentColumn++;
		}
		this->m_currentIndex++;
	}
}

void Lexer::skipComments()
{
	return ;
	if (this->m_inputFileContent[this->m_currentIndex] == '/' && this->m_inputFileContent[this->m_currentIndex + 1] == '/')
	{
		while (this->m_currentIndex < this->m_inputFileContent.size() && this->m_inputFileContent[this->m_currentIndex] != '\n')
		{
			this->m_currentIndex++;
		}
	}
	else if (this->m_inputFileContent[this->m_currentIndex] == '/' && this->m_inputFileContent[this->m_currentIndex + 1] == '*')
	{
		this->m_currentIndex += 2;
		while (this->m_currentIndex < this->m_inputFileContent.size() && (this->m_inputFileContent[this->m_currentIndex] != '*' || this->m_inputFileContent[this->m_currentIndex + 1] != '/'))
		{
			if (this->m_inputFileContent[this->m_currentIndex] == '\n')
			{
				this->m_currentLine++;
				this->m_currentColumn = 0;
			}
			else
			{
				this->m_currentColumn++;
			}
			this->m_currentIndex++;
		}
		this->m_currentIndex += 2;
	}
}