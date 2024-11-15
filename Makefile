# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: stfn <stfn@student.42.fr>                  +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/11/15 23:16:58 by stfn              #+#    #+#              #
#    Updated: 2024/11/16 00:30:58 by stfn             ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = minishell

CC = cc
CFLAGS = -Wall -Wextra -Werror -I./include
LIBFT_DIR = libft
LIBFT = $(LIBFT_DIR)/libft.a

LDFLAGS = -L$(LIBFT_DIR)
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

# Lexer 
SRCS += $(SRC_DIR)/lexer/lexer.c \
        $(SRC_DIR)/lexer/lexer_utils.c \
        $(SRC_DIR)/lexer/lexer_extensions.c \
        $(SRC_DIR)/lexer/lexer_operators.c \
		$(SRC_DIR)/lexer/lexer_tester.c \

# Utils 
SRCS += $(SRC_DIR)/utils/utils.c \

# AST 
SRCS += $(SRC_DIR)/ast/ast.c \

# Parser 
SRCS += $(SRC_DIR)/parser/parser.c

# Object files
OBJ_DIR = obj
OBJS = $(SRCS:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

all: $(NAME)

$(NAME): $(OBJS) $(LIBFT)
	@$(CC) $(CFLAGS) $(OBJS) $(LIBFT) -o $(NAME) $(LDFLAGS) $(LIBS)
	@echo "${COLOR_GREEN}Project compiled successfully.${COLOR_RESET}"

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) -c $< -o $@

$(LIBFT):
	@make -C $(LIBFT_DIR) > /dev/null

clean:
	@rm -rf $(OBJ_DIR)
	@make -C $(LIBFT_DIR) clean > /dev/null
	@echo "${COLOR_YELLOW}Object files and directories cleaned.${COLOR_RESET}"

fclean: clean
	@rm -f $(NAME)
	@make -C $(LIBFT_DIR) fclean > /dev/null
	@echo "${COLOR_YELLOW}Executable and all objects fully cleaned.${COLOR_RESET}"

re: fclean all

.PHONY: all clean fclean re
