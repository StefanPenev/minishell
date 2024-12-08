/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_redirection_utils.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stefan <stefan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/04 22:05:37 by stfn              #+#    #+#             */
/*   Updated: 2024/12/08 23:25:44 by stefan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include "minishell.h"

/* 
 * Helper function to check if a token is a redirection token. This function 
 * checks if the provided token type matches any of the redirection token types, 
 * including output redirection, input redirection, append, and heredoc.
 */
int	is_redirection_token(t_token_type token_type)
{
	return (token_type == TOKEN_REDIRECT_OUT
		|| token_type == TOKEN_REDIRECT_IN
		|| token_type == TOKEN_APPEND
		|| token_type == TOKEN_HEREDOC);
}

/* 
 * Handles the redirection filename. If the current token is not a word (i.e., 
 * not a filename), an error is printed and the redirection object is freed. 
 * If the current token is valid, it duplicates the filename and advances the 
 * parser to the next token. If memory allocation fails, an error is printed.
 */
int	handle_redirection_filename(t_parser *parser, t_redirection *redir)
{
	if (!parser->current_token || parser->current_token->type
		!= TOKEN_WORD || !parser->current_token->value)
	{
		if (parser->current_token && parser->current_token->value)
			ft_print_error("Error: Expected filename after redirection"
				"operator, but found '", parser->current_token->value, "'.\n");
		else
			ft_print_error("Error: Expected filename after redirection"
				"operator, but found 'NULL'.\n", NULL, NULL);
		free(redir);
		return (0);
	}
	redir->filename = ft_strdup(parser->current_token->value);
	if (!redir->filename)
	{
		ft_print_error("Error: Memory allocation failed for filename.\n",
			NULL, NULL);
		free(redir);
		return (0);
	}
	parser_advance(parser);
	return (1);
}
