/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_heredoc_handling.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stefan <stefan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/03 21:24:13 by stfn              #+#    #+#             */
/*   Updated: 2024/12/08 23:08:11 by stefan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include "minishell.h"

/* 
 * Frees a linked list of redirection structures. For each redirection, 
 * its filename (if allocated) is freed, and then the redirection structure 
 * itself is freed.
 */
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

/* 
 * Parses redirection tokens from the parser. For each valid redirection token, 
 * a new redirection structure is parsed and added to the command's redirection 
 * list. Returns 1 if successful, 0 otherwise.
 */
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

/* 
 * Validates a heredoc token by checking if the current token is a valid 
 * heredoc delimiter (a word). If valid, stores the filename (delimiter) 
 * in the redirection structure. If invalid, an error is printed, and 
 * the redirection structure is freed.
 */
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

/* 
 * Handles the heredoc redirection by validating the heredoc delimiter token.
 * If validation fails, returns 0. Otherwise, returns 1 to indicate success.
 */
int	handle_heredoc(t_parser *parser, t_redirection *redir)
{
	if (!validate_heredoc_token(parser, redir))
		return (0);
	return (1);
}

/* 
 * Allocates and initializes a new redirection structure with default values. 
 * If memory allocation fails, an error message is printed and NULL is returned.
 */
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
