#ifndef MESSAGES_H
#define MESSAGES_H

// Genel Hatalar
#define MSG_NO_INPUT_FILE "Error: No input file was provided. Please specify an input file to proceed."
#define MSG_TOKEN_NOT_FOUND "Error: Expected token was not found. Please check the syntax and token sequence."

// Dosya Hataları
#define MSG_COULD_NOT_OPEN_FILE(x) "Error: Unable to open the specified file: '" + std::string(x) + "'. Check if the file exists and is accessible."

// Sözdizim Hataları
#define MSG_INVALID_CHARACTER(x, y, z) "Syntax Error: Invalid character '" + std::string(1, x) + "' detected at line " + std::to_string(y) + ", column " + std::to_string(z) + "."
#define MSG_UNTERMINATED_STRING(x, y, z) "Syntax Error: Unterminated string in file '" + std::string(x) + "' at line " + std::to_string(y) + ", column " + std::to_string(z) + ". Ensure all strings are closed properly."
#define MSG_INVALID_STATEMENT(x) "Error: Unsupported or invalid statement type: " + std::to_string(x) + ". Verify that the statement type is allowed."

// Token Hataları
#define MSG_INVALID_TOKEN_TYPE(x) "Error: Unrecognized token type encountered: " + std::to_string(x) + ". Verify that the token type is valid."

// AST (Abstract Syntax Tree) Hataları
#define MSG_INVALID_AST_NODE_TYPE(x) "Error: Unrecognized AST (Abstract Syntax Tree) node type: " + std::to_string(x) + ". Ensure the node type is correct."
#define MSG_AST_NODE_ASSIGN(varType, varName, value) "AST Assignment: Assigned " + varType + " to variable '" + varName + "' with value " + (value) + "."
#define MSG_EXPECTED_RPAREN "Error: Expected a right parenthesis ')' but found something else. Ensure that the syntax is correct."

#endif
