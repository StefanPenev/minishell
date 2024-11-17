/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_tester.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stfn <stfn@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/16 17:57:34 by stfn              #+#    #+#             */
/*   Updated: 2024/11/17 17:51:30 by stfn             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"
#include "minishell.h"

void	print_misc_tokens(t_token *token)
{
	if (token->type == TOKEN_LEFT_PAREN)
		printf("LEFT PAREN: '%s'\n", token->value);
	else if (token->type == TOKEN_RIGHT_PAREN)
		printf("RIGHT PAREN: '%s'\n", token->value);
	else if (token->type == TOKEN_WILDCARD)
		printf("WILDCARD: '%s'\n", token->value);
	else if (token->type == TOKEN_ERROR)
		printf("ERROR: '%s'\n", token->value);
	else
		printf("UNKNOWN TOKEN: '%s'\n", token->value);
}

void	print_token_type(t_token *token)
{
	if (token->type == TOKEN_WORD)
		printf("WORD: '%s'\n", token->value);
	else if (token->type == TOKEN_PIPE)
		printf("PIPE: '%s'\n", token->value);
	else if (token->type == TOKEN_REDIRECT_IN)
		printf("REDIRECT_IN: '%s'\n", token->value);
	else if (token->type == TOKEN_REDIRECT_OUT)
		printf("REDIRECT_OUT: '%s'\n", token->value);
	else if (token->type == TOKEN_APPEND)
		printf("APPEND: '%s'\n", token->value);
	else if (token->type == TOKEN_HEREDOC)
		printf("HEREDOC: '%s'\n", token->value);
	else if (token->type == TOKEN_AND)
		printf("AND: '%s'\n", token->value);
	else if (token->type == TOKEN_OR)
		printf("OR: '%s'\n", token->value);
	else if (token->type == TOKEN_EOF)
		printf("EOF\n");
	else
		print_misc_tokens(token);
}

void	print_tokens(t_token *tokens)
{
	while (tokens != NULL)
	{
		print_token_type(tokens);
		tokens = tokens->next;
	}
}
