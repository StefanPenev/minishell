/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_operators.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stefan <stefan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/11 23:41:10 by stfn              #+#    #+#             */
/*   Updated: 2024/12/08 21:52:15 by stefan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"

/* 
 * Handles the pipe operator '|'. If the next character is also a pipe, it 
 * creates a TOKEN_OR token for "||", otherwise, it creates a TOKEN_PIPE token 
 * for a single pipe "|".
 */
t_token	*handle_pipe(t_lexer *lexer)
{
	t_token	*token;

	lexer_advance(lexer);
	if (lexer->current_char == '|')
	{
		lexer_advance(lexer);
		token = lexer_new_token(TOKEN_OR, "||");
		if (!token || !token->value)
		{
			if (token)
				free(token->value);
			free(token);
			return (NULL);
		}
		return (token);
	}
	token = lexer_new_token(TOKEN_PIPE, "|");
	if (!token || !token->value)
	{
		lexer_free_tokens(token);
		free(token);
		return (NULL);
	}
	return (token);
}

/* 
 * Handles the ampersand operator '&'. If the next character is also an 
 * ampersand, it creates a TOKEN_AND token for "&&", otherwise, it creates 
 * a TOKEN_ERROR token for a single ampersand "&".
 */
t_token	*handle_ampersand(t_lexer *lexer)
{
	lexer_advance(lexer);
	if (lexer->current_char == '&')
	{
		lexer_advance(lexer);
		return (lexer_new_token(TOKEN_AND, "&&"));
	}
	return (lexer_new_token(TOKEN_ERROR, "&"));
}

/* 
 * Handles the input redirection operator '<'. If the next character is also 
 * a less-than symbol, it creates a TOKEN_HEREDOC token for "<<", otherwise, 
 * it creates a TOKEN_REDIRECT_IN token for a single "<".
 */
t_token	*handle_redirect_in(t_lexer *lexer)
{
	lexer_advance(lexer);
	if (lexer->current_char == '<')
	{
		lexer_advance(lexer);
		return (lexer_new_token(TOKEN_HEREDOC, "<<"));
	}
	return (lexer_new_token(TOKEN_REDIRECT_IN, "<"));
}

/* 
 * Handles the output redirection operator '>'. If the next character is also 
 * a greater-than symbol, it creates a TOKEN_APPEND token for ">>", otherwise, 
 * it creates a TOKEN_REDIRECT_OUT token for a single ">".
 */
t_token	*handle_redirect_out(t_lexer *lexer)
{
	lexer_advance(lexer);
	if (lexer->current_char == '>')
	{
		lexer_advance(lexer);
		return (lexer_new_token(TOKEN_APPEND, ">>"));
	}
	return (lexer_new_token(TOKEN_REDIRECT_OUT, ">"));
}

/* 
 * Handles the wildcard operator '*'. It creates a TOKEN_WILDCARD token for "*".
 */
t_token	*handle_wildcard(t_lexer *lexer)
{
	lexer_advance(lexer);
	return (lexer_new_token(TOKEN_WILDCARD, "*"));
}
