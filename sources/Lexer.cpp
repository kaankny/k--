#include "../includes/Lexer/Lexer.hpp"
#include "../includes/Logger.hpp"
#include "../includes/messages.h"

#include <fstream>
#include <math.h>

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

	bool isFloat = false;
    int decimalPointPosition = -1;

	if (this->m_currentIndex >= this->m_inputFileContent.size())
	{
		this->m_token.type = TOKEN_TYPE_EOF;
		return true;
	}

	char currentChar = this->m_inputFileContent[this->m_currentIndex];

	switch (currentChar)
	{
		case ';':
			this->m_token.type = TOKEN_TYPE_SEMICOLON;
			break;
		case '+':
			this->m_token.type = TOKEN_TYPE_PLUS;
			break;
		case '-':
			if (this->m_currentIndex + 1 < this->m_inputFileContent.size() && this->m_inputFileContent[this->m_currentIndex + 1] == '>')
			{
				this->m_token.type = TOKEN_TYPE_ARROW;
				this->m_currentIndex++;
				this->m_currentColumn++;
			}
			else
			{
				this->m_token.type = TOKEN_TYPE_MINUS;
			}
			break;
		case '*':
			this->m_token.type = TOKEN_TYPE_STAR;
			break;
		case '/':
			this->m_token.type = TOKEN_TYPE_SLASH;
			break;
		case '(':
			this->m_token.type = TOKEN_TYPE_LPAREN;
			break;
		case ')':
			this->m_token.type = TOKEN_TYPE_RPAREN;
			break;
		case '{':
			this->m_token.type = TOKEN_TYPE_LBRACE;
			break;
		case '}':
			this->m_token.type = TOKEN_TYPE_RBRACE;
			break;
		case '%':
			this->m_token.type = TOKEN_TYPE_PERCENT;
			break;
		case ':':
			this->m_token.type = TOKEN_TYPE_COLON;
			break;
		case ',':
			this->m_token.type = TOKEN_TYPE_COMMA;
			break;
		case '&':
			if (this->m_currentIndex + 1 < this->m_inputFileContent.size() && this->m_inputFileContent[this->m_currentIndex + 1] == '&')
			{
				this->m_token.type = TOKEN_TYPE_AND;
				this->m_currentIndex++;
				this->m_currentColumn++;
			}
			else
			{
				Logger::getInstance().log(LogLevel::ERROR, MSG_INVALID_CHARACTER(currentChar, this->m_currentLine, this->m_currentColumn));
				exit(1);
			}
			break;
		case '|':
			if (this->m_currentIndex + 1 < this->m_inputFileContent.size() && this->m_inputFileContent[this->m_currentIndex + 1] == '|')
			{
				this->m_token.type = TOKEN_TYPE_OR;
				this->m_currentIndex++;
				this->m_currentColumn++;
			}
			else
			{
				Logger::getInstance().log(LogLevel::ERROR, MSG_INVALID_CHARACTER(currentChar, this->m_currentLine, this->m_currentColumn));
				exit(1);
			}
			break;
		case '=':
			if (this->m_currentIndex + 1 < this->m_inputFileContent.size() && this->m_inputFileContent[this->m_currentIndex + 1] == '=')
			{
				this->m_token.type = TOKEN_TYPE_EQUALITY;
				this->m_currentIndex++;
				this->m_currentColumn++;
			}
			else
			{
				this->m_token.type = TOKEN_TYPE_EQUAL;
			}
			break;
		case '!':
			if (this->m_currentIndex + 1 < this->m_inputFileContent.size() && this->m_inputFileContent[this->m_currentIndex + 1] == '=')
			{
				this->m_token.type = TOKEN_TYPE_INEQUALITY;
				this->m_currentIndex++;
				this->m_currentColumn++;
			}
			else
			{
				this->m_token.type = TOKEN_TYPE_EXCLAMATION;
			}
			break;
		case '<':
			if (this->m_currentIndex + 1 < this->m_inputFileContent.size() && this->m_inputFileContent[this->m_currentIndex + 1] == '=')
			{
				this->m_token.type = TOKEN_TYPE_LESS_EQUAL;
				this->m_currentIndex++;
				this->m_currentColumn++;
			}
			else
			{
				this->m_token.type = TOKEN_TYPE_LESS;
			}
			break;
		case '>':
			if (this->m_currentIndex + 1 < this->m_inputFileContent.size() && this->m_inputFileContent[this->m_currentIndex + 1] == '=')
			{
				this->m_token.type = TOKEN_TYPE_GREATER_EQUAL;
				this->m_currentIndex++;
				this->m_currentColumn++;
			}
			else
			{
				this->m_token.type = TOKEN_TYPE_GREATER;
			}
			break;

		case '0' ... '9':  // Rakamlar ile başlayanları kontrol et
            this->m_token.intValue = 0;
            this->m_token.floatValue = 0.0f;

            // Sayı okuma döngüsü
            while (this->m_currentIndex < this->m_inputFileContent.size() &&
                   (isdigit(this->m_inputFileContent[this->m_currentIndex]) ||
                    this->m_inputFileContent[this->m_currentIndex] == '.'))
            {
                if (this->m_inputFileContent[this->m_currentIndex] == '.')
                {
                    if (isFloat)  // İkinci bir nokta var, geçersiz sayı
                    {
                        Logger::getInstance().log(LogLevel::ERROR, "Invalid float number format");
                        exit(1);
                    }
                    isFloat = true;
                }
                else
                {
                    if (isFloat)
                    {
                        decimalPointPosition++;
                        this->m_token.floatValue += (this->m_inputFileContent[this->m_currentIndex] - '0') / pow(10, decimalPointPosition);
                    }
                    else
                    {
                        this->m_token.intValue = this->m_token.intValue * 10 + (this->m_inputFileContent[this->m_currentIndex] - '0');
                    }
                }
                this->m_currentIndex++;
                this->m_currentColumn++;
            }
            this->m_currentIndex--;  // Başka bir karakter okunduysa geri git
            this->m_currentColumn--;

            if (isFloat)
            {
                this->m_token.type = TOKEN_TYPE_FLOATLIT;
            }
            else
            {
                this->m_token.type = TOKEN_TYPE_INTLIT;
            }
            break;
		case 'a' ... 'z':
		case 'A' ... 'Z':
			this->m_token.type = TOKEN_TYPE_IDENTIFIER;
			this->m_token.identifier = "";
			while (this->m_currentIndex < this->m_inputFileContent.size() && ((this->m_inputFileContent[this->m_currentIndex] >= 'a' && this->m_inputFileContent[this->m_currentIndex] <= 'z') || (this->m_inputFileContent[this->m_currentIndex] >= 'A' && this->m_inputFileContent[this->m_currentIndex] <= 'Z')))
			{
				this->m_token.identifier += this->m_inputFileContent[this->m_currentIndex];
				this->m_currentIndex++;
				this->m_currentColumn++;
			}
			this->m_currentIndex--;
			this->m_currentColumn--;

			switch (this->m_token.identifier[0])
			{
				case 'a':
					if (this->m_token.identifier == "assign")
						this->m_token.type = TOKEN_TYPE_ASSIGN;
					else if (this->m_token.identifier == "auto")
						this->m_token.type = TOKEN_TYPE_AUTO;
					break;
				case 'w':
					if (this->m_token.identifier == "write")
						this->m_token.type = TOKEN_TYPE_WRITE;
					else if (this->m_token.identifier == "writeln")
						this->m_token.type = TOKEN_TYPE_WRITELN;
					else if (this->m_token.identifier == "while")
						this->m_token.type = TOKEN_TYPE_WHILE;
					break;
				case 'r':
					if (this->m_token.identifier == "read")
						this->m_token.type = TOKEN_TYPE_READ;
					else if (this->m_token.identifier == "return")
						this->m_token.type = TOKEN_TYPE_RETURN;
					break;
				case 'i':
					if (this->m_token.identifier == "int")
						this->m_token.type = TOKEN_TYPE_INT;
					else if (this->m_token.identifier == "if")
						this->m_token.type = TOKEN_TYPE_IF;
					break;
				case 'c':
					if (this->m_token.identifier == "char")
						this->m_token.type = TOKEN_TYPE_CHAR;
					else if (this->m_token.identifier == "call")
						this->m_token.type = TOKEN_TYPE_CALL;
					break;
				case 's':
					if (this->m_token.identifier == "string")
						this->m_token.type = TOKEN_TYPE_STRING;
					break;
				case 'b':
					if (this->m_token.identifier == "bool")
						this->m_token.type = TOKEN_TYPE_BOOL;
					else if (this->m_token.identifier == "break")
						this->m_token.type = TOKEN_TYPE_BREAK;
					break;
				case 't':
					if (this->m_token.identifier == "true")
						this->m_token.type = TOKEN_TYPE_TRUE;
					break;
				case 'f':
					if (this->m_token.identifier == "false")
						this->m_token.type = TOKEN_TYPE_FALSE;
					else if (this->m_token.identifier == "for")
						this->m_token.type = TOKEN_TYPE_FOR;
					else if (this->m_token.identifier == "function")
						this->m_token.type = TOKEN_TYPE_FUNCTION;
					else if (this->m_token.identifier == "float")
						this->m_token.type = TOKEN_TYPE_FLOAT;
					break;
				case 'e':
					if (this->m_token.identifier == "else")
						this->m_token.type = TOKEN_TYPE_ELSE;
					else if (this->m_token.identifier == "elseif")
						this->m_token.type = TOKEN_TYPE_ELSE_IF;
					else if (this->m_token.identifier == "endif")
						this->m_token.type = TOKEN_TYPE_ENDIF;
					else if (this->m_token.identifier == "endwhile")
						this->m_token.type = TOKEN_TYPE_ENDWHILE;
					else if (this->m_token.identifier == "endfor")
						this->m_token.type = TOKEN_TYPE_ENDFOR;
					else if (this->m_token.identifier == "endfunction")
						this->m_token.type = TOKEN_TYPE_ENDFUNCTION;
					break;
				default:
					break;
			}
			break;
		case '"':
			this->m_token.type = TOKEN_TYPE_STRINGLIT;
			this->m_token.stringValue = "";
			this->m_currentIndex++;
			this->m_currentColumn++;
			while (this->m_currentIndex < this->m_inputFileContent.size() && this->m_inputFileContent[this->m_currentIndex] != '"')
			{
				this->m_token.stringValue += this->m_inputFileContent[this->m_currentIndex];
				this->m_currentIndex++;
				this->m_currentColumn++;
			}
			if (this->m_currentIndex >= this->m_inputFileContent.size())
			{
				Logger::getInstance().log(LogLevel::ERROR, MSG_UNTERMINATED_STRING(this->m_inputFileName, this->m_currentLine, this->m_currentColumn));
				exit(1);
			}
			break;
		case '\'':
			this->m_token.type = TOKEN_TYPE_CHARLIT;
			this->m_token.charValue = this->m_inputFileContent[this->m_currentIndex + 1];
			this->m_currentIndex += 2;
			this->m_currentColumn += 2;
			if (this->m_inputFileContent[this->m_currentIndex] != '\'')
			{
				Logger::getInstance().log(LogLevel::ERROR, MSG_UNTERMINATED_STRING(this->m_inputFileName, this->m_currentLine, this->m_currentColumn));
				exit(1);
			}
			break;
		default:
			Logger::getInstance().log(LogLevel::ERROR, MSG_INVALID_CHARACTER(currentChar, this->m_currentLine, this->m_currentColumn));
			exit(1);
	}
	this->m_currentIndex++;
	this->m_currentColumn++;
	return true;
}

void Lexer::skipWhitespaces()
{
	while (this->m_currentIndex < this->m_inputFileContent.size() &&
			(this->m_inputFileContent[this->m_currentIndex] == ' ' || 
			this->m_inputFileContent[this->m_currentIndex] == '\t' || 
			this->m_inputFileContent[this->m_currentIndex] == '\n' ||
			this->m_inputFileContent[this->m_currentIndex] == '\r' ||
			this->m_inputFileContent[this->m_currentIndex] == '\v' ||
			this->m_inputFileContent[this->m_currentIndex] == '\f'))
	{
		if (this->m_inputFileContent[this->m_currentIndex] == '\n')
		{
			this->m_currentLine++;
			this->m_currentColumn = 1;
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
    if (this->m_currentIndex + 1 < this->m_inputFileContent.size())
    {
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
            while (this->m_currentIndex + 1 < this->m_inputFileContent.size() && !(this->m_inputFileContent[this->m_currentIndex] == '*' && this->m_inputFileContent[this->m_currentIndex + 1] == '/'))
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
}
