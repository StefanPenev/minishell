# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: spenev <spenev@student.42.fr>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/11/13 11:24:47 by spenev            #+#    #+#              #
#    Updated: 2023/12/04 10:43:40 by spenev           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = libft.a

CC = gcc

FLAGS = -Wall -Werror -Wextra

SRC = ft_isalpha.c ft_isdigit.c ft_isalnum.c ft_isascii.c ft_isprint.c ft_strlen.c \
	  ft_memset.c ft_bzero.c ft_memcpy.c ft_memmove.c ft_strlcpy.c ft_strlcat.c \
	  ft_toupper.c ft_tolower.c ft_strchr.c ft_strrchr.c ft_strncmp.c ft_memchr.c \
	  ft_memcmp.c ft_strnstr.c ft_atoi.c ft_calloc.c ft_strdup.c ft_strtrim.c ft_itoa.c \
	  ft_substr.c ft_split.c ft_putchar_fd.c ft_putnbr_fd.c ft_putstr_fd.c ft_strmapi.c \
	  ft_putendl_fd.c ft_strjoin.c ft_striteri.c

BSRC = ft_lstadd_back.c ft_lstadd_front.c ft_lstclear.c ft_lstdelone.c ft_lstiter.c \
	   ft_lstlast.c ft_lstnew.c ft_lstsize.c ft_lstmap.c

OBJ = $(SRC:.c=.o)

BOBJ = $(BSRC:.c=.o)

HD = libft.h

.c.o:
	$(CC) $(FLAGS) -c $< -o ${<:.c=.o}

all: $(NAME)

$(NAME): $(OBJ)
	ar rc $(NAME) $(OBJ) 
	ranlib $(NAME)
	
clean:
	rm -f $(OBJ) $(BOBJ)

fclean: clean
	rm -f $(NAME)

bonus: $(BOBJ)
	ar -crs $(NAME) $(BOBJ)

re: fclean all

.PHONY: all bonus clean fclean re 