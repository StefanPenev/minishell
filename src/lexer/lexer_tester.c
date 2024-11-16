/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_tester.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stfn <stfn@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/16 17:57:34 by stfn              #+#    #+#             */
/*   Updated: 2024/11/16 18:05:35 by stfn             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"
#include "minishell.h"

void	print_tokens(t_token *tokens)
{
	while (tokens != NULL)
	{
		switch (tokens->type)
		{
			case TOKEN_WORD: printf("WORD: '%s'\n", tokens->value); break;
			case TOKEN_PIPE: printf("PIPE: '%s'\n", tokens->value); break;
			case TOKEN_REDIRECT_IN: printf("REDIRECT_IN: '%s'\n", tokens->value); break;
			case TOKEN_REDIRECT_OUT: printf("REDIRECT_OUT: '%s'\n", tokens->value); break;
			case TOKEN_APPEND: printf("APPEND: '%s'\n", tokens->value); break;
			case TOKEN_HEREDOC: printf("HEREDOC: '%s'\n", tokens->value); break;
			case TOKEN_AND: printf("AND: '%s'\n", tokens->value); break;
			case TOKEN_OR: printf("OR: '%s'\n", tokens->value); break;
			case TOKEN_EOF: printf("EOF\n"); break;
			case TOKEN_LEFT_PAREN: printf("LEFT PAREN: '%s'\n", tokens->value); break;
			case TOKEN_RIGHT_PAREN: printf("RIGHT PAREN: '%s'\n", tokens->value); break;
			case TOKEN_WILDCARD: printf("WILDCARD: '%s'\n", tokens->value); break;
			case TOKEN_ERROR: printf("ERROR: '%s'\n", tokens->value); break;
			default: printf("UNKNOWN TOKEN: '%s'\n", tokens->value);
		}
		tokens = tokens->next;
	}
}