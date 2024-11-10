#pragma once

#include "token.h"

#include <string>

class Lexer
{
    public:
        static Lexer& getInstance()
        {
            static Lexer instance;
            return instance;
        }

		void init(const char *inputFilePath);

		bool getNextToken();

		t_token *getToken() { return &m_token; }
    private:
        Lexer() {}
        Lexer(Lexer const&) = delete;
        void operator=(Lexer const&) = delete;
		
		/*
			** This is the current token that the lexer is pointing to.
		*/
		t_token m_token;

		std::string m_inputFileName;
		std::string m_inputFileContent;

		size_t m_currentIndex = 0;
		size_t m_currentLine = 1;
		size_t m_currentColumn = 1;

		void skipWhitespaces();
		void skipComments();
};