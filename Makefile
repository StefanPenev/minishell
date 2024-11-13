# Name of the executable
NAME = minishell

# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -Werror -I./include -g
LIBFT_DIR = libft
LIBFT = $(LIBFT_DIR)/libft.a

LDFLAGS = -L$(LIBFT_DIR)

# Flags for the readline library
LIBS = -lreadline -lft

COLOR_RED = \033[31m
COLOR_GREEN = \033[32m
COLOR_BLUE = \033[34m
COLOR_YELLOW = \033[33m
COLOR_RESET = \033[0m

# Source directory
SRC_DIR = src
SRCS = $(SRC_DIR)/main.c \
       $(SRC_DIR)/signals.c \

# Lexer module
SRCS += $(SRC_DIR)/lexer/lexer.c \
        $(SRC_DIR)/lexer/lexer_utils.c \
        $(SRC_DIR)/lexer/lexer_extensions.c \
        $(SRC_DIR)/lexer/lexer_operators.c \

# Utils module
SRCS += $(SRC_DIR)/utils/utils.c \

# AST module
SRCS += $(SRC_DIR)/ast/ast.c \

# Parser module
SRCS += $(SRC_DIR)/parser/parser.c

# Object files
OBJ_DIR = obj
OBJS = $(SRCS:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

# Rule to build the executable
all: $(NAME)
	@echo "${COLOR_GREEN}Project compiled successfully.${COLOR_RESET}"

$(NAME): $(OBJS) ${OBJ_DIR} $(LIBFT)
	@$(CC) $(CFLAGS) $(OBJS) -o $(NAME) $(LDFLAGS) $(LIBS)

# Compile .c files into .o files, maintaining directory structure in obj/
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c $(HEADER)
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) -c $< -o $@

$(LIBFT):
	@make -s -C $(LIBFT_DIR)

# Rule to clean object files
clean:
	@rm -rf $(OBJ_DIR)
	@make -s -C $(LIBFT_DIR) clean
	@echo "${COLOR_YELLOW}Object files and directories cleaned.${COLOR_RESET}"

# Rule to clean object files and executable
fclean: clean
	@rm -f $(NAME)
	@make -s -C $(LIBFT_DIR) fclean
	@echo "${COLOR_YELLOW}Executable and all objects fully cleaned.${COLOR_RESET}"

# Rule to recompile everything from scratch
re: fclean all

.PHONY: all clean fclean re
