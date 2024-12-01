/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_redirection_heredoc.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stfn <stfn@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 10:35:42 by stfn              #+#    #+#             */
/*   Updated: 2024/12/01 00:44:19 by stfn             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include "minishell.h"

int	validate_heredoc_token(t_parser *parser, t_redirection *redir)
{
	if (!parser->current_token || parser->current_token->type != TOKEN_WORD
		|| !parser->current_token->value)
	{
		ft_print_error("Error: Expected heredoc delimiter after '<<', but found ", parser->current_token ? parser->current_token->value : "NULL", ".\n");
		free(redir);
		return (0);
	}
	redir->filename = strdup(parser->current_token->value);
	if (!redir->filename)
	{
		ft_putstr_fd("Error: Memory allocation failed for heredoc delimiter.\n", STDERR_FILENO);
		free(redir);
		return (0);
	}
	parser_advance(parser);
	return (1);
}
