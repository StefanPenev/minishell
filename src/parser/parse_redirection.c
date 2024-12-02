/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_redirection.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stfn <stfn@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 09:56:12 by stfn              #+#    #+#             */
/*   Updated: 2024/12/02 23:50:50 by stfn             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include "minishell.h"

void	append_redirection(t_redirection **head, t_redirection *new_redir)
{
	t_redirection	*last;

	if (!*head)
	{
		*head = new_redir;
	}
	else
	{
		last = *head;
		while (last->next)
			last = last->next;
		last->next = new_redir;
	}
}

int	handle_filename(t_parser *parser, t_redirection *redir)
{
	if (!parser->current_token || parser->current_token->type
		!= TOKEN_WORD || !parser->current_token->value)
	{
		fprintf(stderr, "Error: Expected filename after redirection operator, but found '%s'.\n", parser->current_token && parser->current_token->value ? parser->current_token->value : "NULL");
		free(redir);
		return (0);
	}
	redir->filename = strdup(parser->current_token->value);
	if (!redir->filename)
	{
		fprintf(stderr, "Error: Memory allocation failed for filename.\n");
		free(redir);
		return (0);
	}
	parser_advance(parser);
	return (1);
}

int	handle_heredoc(t_parser *parser, t_redirection *redir)
{
	if (!validate_heredoc_token(parser, redir))
		return (0);
	return (1);
}

t_redirection	*allocate_redirection(void)
{
	t_redirection	*redir;

	redir = malloc(sizeof(t_redirection));
	if (!redir)
	{
		ft_putstr_fd("Error: Memory allocation failed for redirection.\n", STDERR_FILENO);
		return (NULL);
	}
	redir->type = 0;
	redir->filename = NULL;
	redir->heredoc_content = NULL;
	redir->was_processed = 0;
	redir->next = NULL;
	return (redir);
}

t_redirection	*parse_redirection(t_parser *parser)
{
	t_redirection	*redir;

	if (!parser->current_token
		|| !is_redirection_token(parser->current_token->type))
		return (NULL);

	redir = allocate_redirection();
	if (!redir)
		return (NULL);
	map_token_to_redirection(parser->current_token->type, &redir->type);
	parser_advance(parser);
	if (redir->type == HEREDOC)
	{
		if (!handle_heredoc(parser, redir))
			return (NULL);
	}
	else
	{
		if (!handle_filename(parser, redir))
			return (NULL);
	}
	return (redir);
}
