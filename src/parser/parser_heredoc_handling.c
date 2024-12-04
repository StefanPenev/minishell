/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_heredoc_handling.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stfn <stfn@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/03 21:24:13 by stfn              #+#    #+#             */
/*   Updated: 2024/12/04 22:08:02 by stfn             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include "minishell.h"

void	free_redirections(t_redirection *redir)
{
	t_redirection	*next;

	while (redir)
	{
		next = redir->next;
		if (redir->filename)
			free(redir->filename);
		free(redir);
		redir = next;
	}
}

int	parse_redirections(t_parser *parser, t_command *cmd)
{
	t_redirection	*redir;

	while (parser->current_token
		&& is_redirection_token(parser->current_token->type))
	{
		redir = parse_redirection(parser);
		if (!redir)
			return (0);
		redir->next = cmd->redirections;
		cmd->redirections = redir;
	}
	return (1);
}

int	validate_heredoc_token(t_parser *parser, t_redirection *redir)
{
	if (!parser->current_token || parser->current_token->type != TOKEN_WORD
		|| !parser->current_token->value)
	{
		if (parser->current_token)
			ft_print_error("Error: Expected heredoc delimiter after '<<',"
				"but found ", parser->current_token->value, ".\n");
		else
			ft_print_error("Error: Expected heredoc delimiter after '<<',"
				"but found NULL.\n", NULL, NULL);
		free(redir);
		return (0);
	}
	redir->filename = strdup(parser->current_token->value);
	if (!redir->filename)
	{
		ft_putstr_fd("Error: Memory allocation failed for heredoc delimiter.\n",
			STDERR_FILENO);
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
		ft_putstr_fd("Error: Memory allocation failed for redirection.\n",
			STDERR_FILENO);
		return (NULL);
	}
	redir->type = 0;
	redir->filename = NULL;
	redir->heredoc_content = NULL;
	redir->was_processed = 0;
	redir->next = NULL;
	return (redir);
}
