NAME = kaan
CC = c++
CFLAGS = -DDEBUGMODE #-Wall -Wextra -Werror
RM = rm -f

SRC = sources/main.cpp \
	  sources/Lexer.cpp \
	  sources/Parser.cpp

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