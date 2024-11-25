NAME = kaan
CC = c++
CFLAGS = -DDEBUGMODE #-Wall -Wextra -Werror
RM = rm -f

# Renk Tanımları
ECHO = echo
RESET = \e[0m
RED = \e[31m
GREEN = \e[32m
YELLOW = \e[33m
BLUE = \e[34m
MAGENTA = \e[35m
CYAN = \e[36m
WHITE = \e[37m

# Kaynak Dosyalar
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

# Toplam Kaynak Sayısı
TOTAL_FILES := $(words $(SRC))

all: reset_count $(NAME)

# Derleme İlerleme Sayacını Sıfırla
reset_count:
	@echo 0 > .count

$(NAME): $(OBJ)
	@$(ECHO) "$(YELLOW)+----------------------------------------+$(RESET)"
	@$(ECHO) "$(GREEN)Linking object files to create $(CYAN)$(NAME)$(RESET)"
	@$(CC) $(CFLAGS) -o $(NAME) $(OBJ)
	@$(ECHO) "$(MAGENTA)Build complete!$(RESET)"
	@$(ECHO) "$(YELLOW)+----------------------------------------+$(RESET)"
	@$(RM) .count

# Derleme İşlemi için Özel Kural
define COMPILE_MESSAGE
	@COUNT=$$(cat .count); \
	COUNT=$$(($$COUNT + 1)); \
	echo $$COUNT > .count; \
	PROGRESS=$$(($$COUNT * 100 / $(TOTAL_FILES))); \
	$(ECHO) "$(YELLOW)+----------------------------------------+$(RESET)"; \
	$(ECHO) "| $(BLUE)Compiling $(CYAN)$<$(RESET)"; \
	$(CC) $(CFLAGS) -c $< -o $@; \
	$(ECHO) "| $(GREEN)Compiled $(CYAN)$<$(RESET) - $$PROGRESS%% completed"; \
	$(ECHO) "$(YELLOW)+----------------------------------------+$(RESET)"
endef

%.o: %.cpp
	$(COMPILE_MESSAGE)

clean:
	@$(ECHO) "$(RED)Cleaning object files...$(RESET)"
	@$(RM) $(OBJ) .count

fclean: clean
	@$(ECHO) "$(RED)Removing executable $(CYAN)$(NAME)$(RESET)"
	@$(RM) $(NAME)

re: fclean all

.PHONY: all clean fclean re reset_count
