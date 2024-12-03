/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stfn <stfn@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/13 13:31:41 by stfn              #+#    #+#             */
/*   Updated: 2024/12/03 01:30:31 by stfn             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

// Forward declarations are essential when a type or struct
// is referenced before its full definition is available in
// the current scope. These declarations inform the compiler
// about the existence of a specific type, enabling its use in
// function prototypes, pointers, or struct members without
// requiring immediate knowledge of its complete definition.

// The errors occurred when I attempted to include
// t_shell_context in the pipes module, likely because its
// full definition was not yet visible in the scope where it
// was being used. By adding forward declarations for
// t_shell_context and other types, the compiler can process
// the references correctly without requiring the complete details
// of their implementation upfront. This approach resolves such
// dependency issues.

# include "../libft/libft.h"
# include "ast.h"
# include "lexer.h"
# include "process.h"
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <readline/readline.h>
# include <readline/history.h>

typedef struct s_token		t_token; // anna
typedef struct s_lexer		t_lexer; // anna
typedef struct s_process	t_process; // anna
typedef struct s_env		t_env; // anna

typedef struct s_shell_context
{
	t_process	*process;
	t_env		*env_copy;
}				t_shell_context;

void			print_tokens(t_token *token);
void			print_ast(t_ast *ast, int indent);

// main_utils.c
t_ast			*process_parser(t_token *tokens);
void			cleanup(t_token *tokens, char *input);
t_token			*process_lexer(char *input, t_shell_context **shell_ctx);
t_token			*lexer_tokenize(char *input, t_shell_context **shell_ctx);
t_token			*lexer_process_token(t_lexer *lexer, t_token *head,
					t_shell_context **shell_ctx);
t_token			*lexer_handle_dollar(t_lexer *lexer, t_token *head,
					t_shell_context **shell_ctx);
void			free_shell_ctx(t_shell_context *shell_ctx);

void			ft_print_error(char *prefix, char *dynamic_part, char *suffix);
char			*ft_strcat(char *dest, char *src);
void			*ft_realloc(void *ptr, size_t new_size);
char			*ft_strcpy(char *dest, char *src);

char *lexer_expand_dollar(t_lexer *lexer, t_token *head, t_shell_context **shell_ctx);
t_token *lexer_handle_word(t_lexer *lexer, t_shell_context **shell_ctx);
char *lexer_collect_word(t_lexer *lexer, t_shell_context **shell_ctx);
char *lexer_collect_subword(t_lexer *lexer, t_shell_context **shell_ctx);
char *lexer_collect_quoted(t_lexer *lexer, char quote_type, t_shell_context **shell_ctx);
t_token				*lexer_handle_quotes(t_lexer *lexer, t_shell_context **shell_ctx);

#endif
