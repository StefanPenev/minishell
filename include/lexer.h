/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stfn <stfn@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/12 00:52:54 by stfn              #+#    #+#             */
/*   Updated: 2024/12/03 20:03:49 by stfn             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LEXER_H
# define LEXER_H

# include "process.h"
# include <ctype.h>
# include <dirent.h> // For wildcard expansion (manual)
# include <stddef.h>

# define INITIAL_CAPACITY 128

// explanations is in minishell.h
typedef struct s_env	t_env; // anna

typedef enum e_token_type
{
	TOKEN_WORD,
	TOKEN_PIPE,
	TOKEN_REDIRECT_IN,
	TOKEN_REDIRECT_OUT,
	TOKEN_APPEND,
	TOKEN_HEREDOC,
	TOKEN_AND,
	TOKEN_OR,
	TOKEN_LEFT_PAREN,
	TOKEN_RIGHT_PAREN,
	TOKEN_EOF,
	TOKEN_WILDCARD,
	TOKEN_ERROR
}					t_token_type;

typedef struct s_token
{
	t_token_type	type;
	char			*value;
	struct s_token	*next;
}					t_token;

typedef struct s_lexer
{
	char			*input;
	size_t			pos;
	char			current_char;
	t_env			*envp;
	int				last_exit_status;
}					t_lexer;

// lexer.c
void				lexer_free_tokens(t_token *tokens);
t_token				*lexer_handle_operator(t_lexer *lexer);
const char			*lexer_get_error_message(t_token *token);
void				lexer_append_token(t_token **head, t_token **current,
						t_token *new_tok);

// lexer_utils.c
int					is_special_char(char c);
void				lexer_advance(t_lexer *lexer);
void				lexer_free_tokens(t_token *head);
void				lexer_skip_whitespace(t_lexer *lexer);
const char			*lexer_get_error_message(t_token *token);

// lexer_extensions.c
//char				*lexer_collect_word(t_lexer *lexer);
t_token				*lexer_new_token(t_token_type type, char *value);
char				*lexer_expand_variable(t_lexer *lexer, size_t *length,
						t_env *env_copy, int last_exit_status);

// lexer_operators.c
t_token				*handle_pipe(t_lexer *lexer);
t_token				*handle_wildcard(t_lexer *lexer);
t_token				*handle_ampersand(t_lexer *lexer);
t_token				*handle_redirect_in(t_lexer *lexer);
t_token				*handle_redirect_out(t_lexer *lexer);

t_token				*lexer_finalize_tokens(t_token *head, t_token **current);

// lexer_wildcard.c
char				**expand_wildcard(void);

// lexer_wildcard_utils.c
void				free_files(char **files, int size);
int					resize_array(char ***files, int *capacity,
						int current_size);

// lexer_env_variables.c
char				*ft_getenv(char *var_name, t_env *env_copy);

// utils.c
int					ft_isspace(int c);
char				*ft_strndup(const char *s, size_t n);

#endif
