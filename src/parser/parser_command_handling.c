/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_command_handling.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stefan <stefan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/03 21:21:37 by stfn              #+#    #+#             */
/*   Updated: 2024/12/08 23:10:46 by stefan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ast.h"
#include "parser.h"
#include "minishell.h"

/* 
 * Validates a command by checking if there are arguments or redirections. 
 * Returns 1 if valid, 0 otherwise.
 */
int	validate_command(t_command *cmd, size_t args_size)
{
	return (!(args_size == 0 && !cmd->redirections));
}

/* 
 * Initializes and collects command arguments, ensuring enough capacity and 
 * handling wildcards. If any step fails, frees resources and returns 0.
 */
int	initialize_and_collect_arguments(t_parser *parser, t_ast *node,
	t_command *cmd)
{
	size_t	args_size;
	size_t	args_capacity;

	args_size = 0;
	args_capacity = 10;
	if (!initialize_arguments(cmd, &args_capacity))
	{
		free(node);
		command_free(cmd);
		return (0);
	}
	if (!collect_arguments(parser, cmd, &args_size, &args_capacity))
	{
		free(node);
		command_free(cmd);
		return (0);
	}
	handle_wildcards(parser, cmd);
	if (!validate_command(cmd, args_size))
	{
		free(node);
		command_free(cmd);
		return (0);
	}
	return (1);
}

/* 
 * Processes redirections for the command. Parses and adds them to the command 
 * if valid. Frees resources if any redirection parsing fails.
 */
int	process_command_redirections(t_parser *parser, t_ast *node, t_command *cmd)
{
	t_redirection	*redir;

	while (parser->current_token
		&& is_redirection_token(parser->current_token->type))
	{
		redir = parse_redirection(parser);
		if (!redir)
		{
			free(node);
			command_free(cmd);
			return (0);
		}
		add_redirection_to_command(cmd, redir);
	}
	return (1);
}

/* 
 * Parses a command, initializing the command node and processing its arguments 
 * and redirections. Returns the populated AST node if successful, NULL otherwise.
 */
t_ast	*parse_command(t_parser *parser)
{
	t_ast			*node;
	t_command		*cmd;

	node = allocate_and_initialize_node();
	if (!node)
		return (NULL);
	cmd = allocate_and_initialize_command();
	if (!cmd)
	{
		free(node);
		return (NULL);
	}
	if (!initialize_and_collect_arguments(parser, node, cmd))
		return (NULL);
	if (!process_command_redirections(parser, node, cmd))
		return (NULL);
	node->u_data.command = cmd;
	return (node);
}
