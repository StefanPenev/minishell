/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_operators.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stfn <stfn@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/11 23:41:10 by stfn              #+#    #+#             */
/*   Updated: 2024/11/16 00:14:13 by stfn             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"

/* Handle pipe operator: | and || */
t_token	*handle_pipe(t_lexer *lexer)
{
	lexer_advance(lexer);
	if (lexer->current_char == '|')
	{
		lexer_advance(lexer);
		return (lexer_new_token(TOKEN_OR, "||"));
	}
	return (lexer_new_token(TOKEN_PIPE, "|"));
}

/* Handle ampersand operator: & and && */
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

/* Handle input redirection operator: < and << */
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

/* Handle output redirection operator: > and >> */
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

/* Handle wildcard operator: * */
t_token	*handle_wildcard(t_lexer *lexer)
{
	lexer_advance(lexer);
	return (lexer_new_token(TOKEN_WILDCARD, "*"));
}
