/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_redirection_utils.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stfn <stfn@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/04 22:05:37 by stfn              #+#    #+#             */
/*   Updated: 2024/12/04 22:25:06 by stfn             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include "minishell.h"

// Helper function to check if a token is a redirection token
int	is_redirection_token(t_token_type token_type)
{
	return (token_type == TOKEN_REDIRECT_OUT
		|| token_type == TOKEN_REDIRECT_IN
		|| token_type == TOKEN_APPEND
		|| token_type == TOKEN_HEREDOC);
}

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
	redir->filename = strdup(parser->current_token->value);
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
