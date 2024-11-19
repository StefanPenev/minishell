# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: anilchen <anilchen@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/08/09 14:53:42 by anilchen          #+#    #+#              #
#    Updated: 2024/11/15 18:09:27 by anilchen         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = minishell
CC = cc
CFLAGS = -g -I$(LIBFT_DIR) #-Wall -Wextra -Werror -fsanitize=address 

LIBFT_DIR = libft
LIBFT_NAME = $(LIBFT_DIR)/libft.a

SRC_DIR = src
OBJ_DIR = obj

READLINE_FLAGS = -lreadline
SRCS = $(wildcard $(SRC_DIR)/*.c)
OBJ = $(SRCS:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

MSG_LIBFT_CREATED = "Libft successfully created."
MSG_EXEC_CREATED = "Executable successfully created."
MSG_CLEAN = "Cleaning up..."
MSG_FCLEAN = "Full clean up..."

all: $(LIBFT_NAME) $(NAME)

$(LIBFT_NAME):
	@$(MAKE) -C $(LIBFT_DIR) 
	@echo $(MSG_LIBFT_CREATED)
	
$(NAME): $(OBJ) $(LIBFT_NAME)
	$(CC) $(CFLAGS) $(OBJ) $(LIBFT_NAME) $(READLINE_FLAGS) -o $(NAME)
	@echo $(MSG_EXEC_CREATED)


$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@ 

clean:
	rm -rf $(OBJ_DIR)
	$(MAKE) -C $(LIBFT_DIR) clean
	echo $(MSG_CLEAN)

fclean: clean
	rm -f $(NAME)
	$(MAKE) -C $(LIBFT_DIR) fclean
	echo $(MSG_FCLEAN)

re: fclean all

.PHONY: all clean fclean re
