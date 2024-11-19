/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stfn <stfn@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/13 13:31:41 by stfn              #+#    #+#             */
/*   Updated: 2024/11/19 19:04:32 by stfn             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include "../libft/libft.h"
# include "lexer.h"
# include "ast.h"

void	print_tokens(t_token *token);
void	print_ast(t_ast *ast, int indent);

//main_utils.c
t_ast	*process_parser(t_token *tokens);
void	cleanup(t_token *tokens, char *input);
t_token	*process_lexer(char *input, char **envp);

#endif
