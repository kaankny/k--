#ifndef MESSAGES_H
#define MESSAGES_H

#define MSG_NO_INPUT_FILE "No input file provided."
#define MSG_COULD_NOT_OPEN_FILE(x) "Could not open file: " + std::string(x)
#define MSG_INVALID_CHARACTER(x, y, z) "Invalid character '" + std::string(1, x) + "' at line " + std::to_string(y) + ", column " + std::to_string(z)
#define MSG_INVALID_AST_NODE_TYPE(x) "Invalid AST node type: " + std::to_string(x)

#endif