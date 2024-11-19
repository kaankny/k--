NAME = kaan
CC = c++
CFLAGS = -DDEBUGMODE #-Wall -Wextra -Werror
RM = rm -f

SRC = sources/main.cpp \
	  sources/Lexer.cpp \
	  sources/Parser/Parser.cpp \
	  sources/Parser/Statements/parseAssignStatement.cpp \
	  sources/Parser/Statements/parseBreakStatement.cpp \
	  sources/Parser/Statements/parseContinueStatement.cpp \
	  sources/Parser/Statements/parseForStatement.cpp \
	  sources/Parser/Statements/parseFunctionCall.cpp \
	  sources/Parser/Statements/parseFunctionDefinition.cpp \
	  sources/Parser/Statements/parseIfStatement.cpp \
	  sources/Parser/Statements/parseReadStatement.cpp \
	  sources/Parser/Statements/parseReturnStatement.cpp \
	  sources/Parser/Statements/parseSwitchStatement.cpp \
	  sources/Parser/Statements/parseVariableAssign.cpp \
	  sources/Parser/Statements/parseWhileStatement.cpp \
	  sources/Parser/Statements/parseWriteStatement.cpp \
	  sources/Interpreter.cpp \

OBJ = $(SRC:.cpp=.o)

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJ)

clean:
	$(RM) $(OBJ)

fclean: clean
	$(RM) $(NAME)

re: fclean all

.PHONY: all clean fclean re