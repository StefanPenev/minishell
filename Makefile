# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: spenev <spenev@student.42.fr>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/11/15 23:16:58 by stfn              #+#    #+#              #
#    Updated: 2024/12/04 11:44:39 by spenev           ###   ########.fr        #
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
SRCS = $(SRC_DIR)/ast_tester.c \
       
# Main
SRCS += $(SRC_DIR)/main/main.c \
		$(SRC_DIR)/main/main_utils.c \
		$(SRC_DIR)/main/main_cleanup.c \
		$(SRC_DIR)/main/main_workflow.c \

# Lexer 
SRCS += $(SRC_DIR)/lexer/lexer.c \
        $(SRC_DIR)/lexer/lexer_utils.c \
        $(SRC_DIR)/lexer/lexer_tokens.c \
		$(SRC_DIR)/lexer/lexer_wildcard.c \
        $(SRC_DIR)/lexer/lexer_operators.c \
		$(SRC_DIR)/lexer/lexer_extensions.c \
		$(SRC_DIR)/lexer/lexer_env_variables.c \
		$(SRC_DIR)/lexer/lexer_wildcard_utils.c \
		$(SRC_DIR)/lexer/lexer_tester.c \

# Utils 
SRCS += $(SRC_DIR)/utils/utils.c \
		$(SRC_DIR)/utils/errors.c \

# AST 
SRCS += $(SRC_DIR)/ast/ast.c \

# Parser 
SRCS += $(SRC_DIR)/parser/parse_expression.c \
		$(SRC_DIR)/parser/parser_heredoc_handling.c \
		$(SRC_DIR)/parser/parser_command_handling.c \
		$(SRC_DIR)/parser/parser_argument_handling.c \
		$(SRC_DIR)/parser/parser_redirection_handling.c \

# Execution
SRCS += $(SRC_DIR)/execution/getenv.c \
		$(SRC_DIR)/execution/exit_status.c \
		$(SRC_DIR)/execution/minishell_utils.c \
		$(SRC_DIR)/execution/minishell_utils_2.c \
		$(SRC_DIR)/execution/buildins/minishell_pwd.c \
		$(SRC_DIR)/execution/buildins/minishell_exit.c \
		$(SRC_DIR)/execution/buildins/minishell_env.c \
		$(SRC_DIR)/execution/buildins/minishell_export.c \
		$(SRC_DIR)/execution/buildins/minishell_export_2.c \
		$(SRC_DIR)/execution/buildins/minishell_unset.c \
		$(SRC_DIR)/execution/buildins/minishell_cd.c \
		$(SRC_DIR)/execution/buildins/minishell_echo.c \
		$(SRC_DIR)/execution/external_commands.c \
		$(SRC_DIR)/execution/handle_path.c \
		$(SRC_DIR)/execution/pipeline/pipes_main.c \
		$(SRC_DIR)/execution/pipeline/pipes_main_2.c \
		$(SRC_DIR)/execution/pipeline/pipes_commands.c \
		$(SRC_DIR)/execution/pipeline/pipes_utils.c \
		$(SRC_DIR)/execution/pipeline/pipes_utils_2.c \
		$(SRC_DIR)/execution/pipeline/pipes_utils_3.c \
		$(SRC_DIR)/execution/redirections.c \
		$(SRC_DIR)/execution/redirections_2.c \
		$(SRC_DIR)/execution/heredoc.c \
		$(SRC_DIR)/execution/!cleanup.c \

# Signals
SRCS += $(SRC_DIR)/signals/signals.c \
		$(SRC_DIR)/signals/terminal_sig_utils.c \

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

leaks: $(NAME)
	@echo "${COLOR_BLUE}Running valgrind for memory leak check...${COLOR_RESET}"
	@valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes \
		--log-file=valgrind_report.log ./$(NAME)
	@echo "${COLOR_GREEN}Valgrind completed. Check valgrind_report.log for details.${COLOR_RESET}"

re: fclean all

.PHONY: all clean fclean re
