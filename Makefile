# Name of the executable
NAME = minishell

# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -Werror -I./include -g
LIBFT_DIR = libft
LIBFT = $(LIBFT_DIR)/libft.a

# Flags for the readline library
LDFLAGS = -L/usr/local/opt/readline/lib -L$(LIBFT_DIR)
CPPFLAGS = -I/usr/local/opt/readline/include -I$(LIBFT_DIR)
LIBS = -lreadline -lft

# Source files
SRCS = main.c signals.c lexer.c lexer_utils.c lexer_extensions.c lexer_operators.c utils.c

# Object files
OBJS = $(SRCS:.c=.o)

# Rule to build the executable
all: $(NAME)

$(NAME): $(OBJS) $(LIBFT)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME) $(LDFLAGS) $(LIBS) $(CPPFLAGS)

$(LIBFT):
	make -C $(LIBFT_DIR)

# Rule to clean object files
clean:
	rm -f $(OBJS)
	make -C $(LIBFT_DIR) clean

# Rule to clean object files and executable
fclean: clean
	rm -f $(NAME)
	make -C $(LIBFT_DIR) fclean

# Rule to recompile everything from scratch
re: fclean all

.PHONY: all clean fclean re
