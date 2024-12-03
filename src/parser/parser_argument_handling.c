/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_argument_handling.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stfn <stfn@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/03 21:10:10 by stfn              #+#    #+#             */
/*   Updated: 2024/12/03 21:16:37 by stfn             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ast.h"
#include "parser.h"
#include "minishell.h"

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

int	initialize_arguments(t_command *cmd, size_t *args_capacity)
{
	cmd->args = malloc(sizeof(char *) * *args_capacity);
	return (cmd->args != NULL);
}

void	add_argument_to_command(t_command *command, const char *arg)
{
	size_t	args_count;
	size_t	new_capacity;

	args_count = 0;
	while (command->args[args_count] != NULL)
		args_count++;
	if (args_count >= command->args_capacity - 1)
	{
		new_capacity = command->args_capacity * 2;
		command->args = realloc(command->args, new_capacity * sizeof(char *));
		if (!command->args)
		{
			ft_putstr_fd("Failed to allocate memory for arguments\n",
				STDERR_FILENO);
			exit(EXIT_FAILURE);
		}
		command->args_capacity = new_capacity;
	}
	command->args[args_count] = strdup(arg);
	command->args[args_count + 1] = NULL;
}

void	parse_wildcard(t_token *token, t_command *command)
{
	int		i;
	char	**expanded_files;

	if (token->type == TOKEN_WILDCARD)
	{
		expanded_files = expand_wildcard();
		if (!expanded_files)
		{
			ft_putstr_fd("Wildcard expansion failed\n", STDERR_FILENO);
			return ;
		}
		i = 0;
		while (command->args[i] != NULL)
		{
			add_argument_to_command(command, expanded_files[i]);
			free(expanded_files[i]);
			i++;
		}
		free(expanded_files);
	}
}

void	handle_wildcards(t_parser *parser, t_command *cmd)
{
	if (parser->current_token && parser->current_token->type == TOKEN_WILDCARD)
	{
		parse_wildcard(parser->current_token, cmd);
		parser_advance(parser);
	}
}
