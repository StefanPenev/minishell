/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_command_handling.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stfn <stfn@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/03 21:21:37 by stfn              #+#    #+#             */
/*   Updated: 2024/12/03 21:23:53 by stfn             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ast.h"
#include "parser.h"
#include "minishell.h"

// Advance the parser to the next token
void	parser_advance(t_parser *parser)
{
	if (!parser->current_token || parser->current_token->type == TOKEN_EOF
		|| parser->current_token->type == TOKEN_ERROR)
	{
		return ;
	}
	parser->current_token = parser->current_token->next;
}

t_ast	*allocate_and_initialize_node(void)
{
	t_ast	*node;

	node = malloc(sizeof(t_ast));
	if (!node)
		return (NULL);
	node->type = AST_COMMAND;
	return (node);
}

t_command	*allocate_and_initialize_command(void)
{
	t_command	*cmd;

	cmd = malloc(sizeof(t_command));
	if (!cmd)
		return (NULL);
	cmd->args = NULL;
	cmd->redirections = NULL;
	return (cmd);
}

int	validate_command(t_command *cmd, size_t args_size)
{
	return (!(args_size == 0 && !cmd->redirections));
}

t_ast	*parse_command(t_parser *parser)
{
	t_ast			*node;
	t_command		*cmd;
	size_t			args_size;
	size_t			args_capacity;
	t_redirection	*redir;

	node = allocate_and_initialize_node();
	if (!node)
		return (NULL);
	cmd = allocate_and_initialize_command();
	if (!cmd)
	{
		free(node);
		return (NULL);
	}
	args_size = 0;
	args_capacity = 10;
	if (!initialize_arguments(cmd, &args_capacity))
	{
		free(node);
		command_free(cmd);
		return (NULL);
	}
	if (!collect_arguments(parser, cmd, &args_size, &args_capacity))
	{
		free(node);
		command_free(cmd);
		return (NULL);
	}
	handle_wildcards(parser, cmd);
	while (parser->current_token
		&& is_redirection_token(parser->current_token->type))
	{
		redir = parse_redirection(parser);
		if (!redir)
		{
			free(node);
			command_free(cmd);
			return (NULL);
		}
		add_redirection_to_command(cmd, redir);
	}
	if (!validate_command(cmd, args_size))
	{
		free(node);
		command_free(cmd);
		return (NULL);
	}
	node->u_data.command = cmd;
	return (node);
}