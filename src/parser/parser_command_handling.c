/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_command_handling.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stfn <stfn@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/03 21:21:37 by stfn              #+#    #+#             */
/*   Updated: 2024/12/04 22:26:38 by stfn             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ast.h"
#include "parser.h"
#include "minishell.h"

int	validate_command(t_command *cmd, size_t args_size)
{
	return (!(args_size == 0 && !cmd->redirections));
}

// t_ast	*parse_command(t_parser *parser)
// {
// 	t_ast			*node;
// 	t_command		*cmd;
// 	size_t			args_size;
// 	size_t			args_capacity;
// 	t_redirection	*redir;

// 	node = allocate_and_initialize_node();
// 	if (!node)
// 		return (NULL);
// 	cmd = allocate_and_initialize_command();
// 	if (!cmd)
// 	{
// 		free(node);
// 		return (NULL);
// 	}
// 	args_size = 0;
// 	args_capacity = 10;
// 	if (!initialize_arguments(cmd, &args_capacity))
// 	{
// 		free(node);
// 		command_free(cmd);
// 		return (NULL);
// 	}
// 	if (!collect_arguments(parser, cmd, &args_size, &args_capacity))
// 	{
// 		free(node);
// 		command_free(cmd);
// 		return (NULL);
// 	}
// 	handle_wildcards(parser, cmd);
// 	while (parser->current_token
// 		&& is_redirection_token(parser->current_token->type))
// 	{
// 		redir = parse_redirection(parser);
// 		if (!redir)
// 		{
// 			free(node);
// 			command_free(cmd);
// 			return (NULL);
// 		}
// 		add_redirection_to_command(cmd, redir);
// 	}
// 	if (!validate_command(cmd, args_size))
// 	{
// 		free(node);
// 		command_free(cmd);
// 		return (NULL);
// 	}
// 	node->u_data.command = cmd;
// 	return (node);
// }

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
