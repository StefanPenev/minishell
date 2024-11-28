/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_command.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stfn <stfn@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/24 15:26:03 by stfn              #+#    #+#             */
/*   Updated: 2024/11/28 18:32:58 by stfn             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include "minishell.h"
#include "ast.h"

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

int	initialize_arguments(t_command *cmd, size_t *args_capacity)
{
	cmd->args = malloc(sizeof(char *) * *args_capacity);
	return (cmd->args != NULL);
}

int	collect_arguments(t_parser *parser, t_command *cmd,
	size_t *args_size, size_t *args_capacity)
{
	size_t	i;
	char	**new_args;

	while (parser->current_token && parser->current_token->type == TOKEN_WORD)
	{
		if (*args_size >= *args_capacity - 1)
		{
			*args_capacity *= 2;
			new_args = ft_realloc(cmd->args, sizeof(char *) * *args_capacity);
			i = 0;
			if (!new_args)
			{
				while (i < *args_size)
					free(cmd->args[i++]);
				free(cmd->args);
				return (0);
			}
			cmd->args = new_args;
		}
		cmd->args[*args_size] = ft_strdup(parser->current_token->value);
		i = 0;
		if (!cmd->args[*args_size])
		{
			while (i < *args_size)
				free(cmd->args[i++]);
			free(cmd->args);
			return (0);
		}
		(*args_size)++;
		parser_advance(parser);
	}
	cmd->args[*args_size] = NULL;
	return (1);
}

void	handle_wildcards(t_parser *parser, t_command *cmd)
{
	if (parser->current_token && parser->current_token->type == TOKEN_WILDCARD)
	{
		parse_wildcard(parser->current_token, cmd);
		parser_advance(parser);
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

int	validate_command(t_command *cmd, size_t args_size)
{
	return (!(args_size == 0 && !cmd->redirections));
}

void	add_redirection_to_command(t_command *command, t_redirection *new_redir)
{
	t_redirection	*current;

	if (!command->redirections)
	{
		command->redirections = new_redir;
		return ;
	}
	current = command->redirections;
	while (current)
	{
		if (current->type == new_redir->type)
		{
			free(current->filename);
			free(current);
			new_redir->next = command->redirections->next;
			command->redirections = new_redir;
			return ;
		}
		if (!current->next)
			break ;
		current = current->next;
	}
	new_redir->next = NULL;
	current->next = new_redir;
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
