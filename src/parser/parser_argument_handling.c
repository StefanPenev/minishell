/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_argument_handling.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stefan <stefan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/03 21:10:10 by stfn              #+#    #+#             */
/*   Updated: 2024/12/08 23:24:29 by stefan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ast.h"
#include "parser.h"
#include "minishell.h"

/* 
 * Resizes the argument array by doubling its capacity. If memory allocation 
 * fails, frees all previously allocated arguments and returns 0.
 */
int	resize_argument_array(t_command *cmd, size_t *args_size,
	size_t *args_capacity)
{
	char	**new_args;
	size_t	i;

	*args_capacity *= 2;
	new_args = ft_realloc(cmd->args, sizeof(char *) * (*args_capacity));
	if (!new_args)
	{
		i = 0;
		while (i < *args_size)
			free(cmd->args[i++]);
		free(cmd->args);
		return (0);
	}
	cmd->args = new_args;
	return (1);
}

/* 
 * Adds an argument to the command's argument array. If memory allocation 
 * fails, frees all previously allocated arguments and returns 0.
 */
int	add_argument(t_command *cmd, const char *value,
	size_t *args_size)
{
	size_t	i;

	cmd->args[*args_size] = ft_strdup(value);
	if (!cmd->args[*args_size])
	{
		i = 0;
		while (i < *args_size)
			free(cmd->args[i++]);
		free(cmd->args);
		return (0);
	}
	(*args_size)++;
	return (1);
}

/* 
 * Collects arguments for the command by iterating through the tokens. If the 
 * argument array exceeds capacity, it is resized. Returns 1 on success, 0 on 
 * failure.
 */
int	collect_arguments(t_parser *parser, t_command *cmd,
	size_t *args_size, size_t *args_capacity)
{
	while (parser->current_token && parser->current_token->type == TOKEN_WORD)
	{
		if (*args_size >= *args_capacity - 1)
		{
			if (!resize_argument_array(cmd, args_size, args_capacity))
				return (0);
		}
		if (!add_argument(cmd, parser->current_token->value,
				args_size))
			return (0);
		parser_advance(parser);
	}
	cmd->args[*args_size] = NULL;
	return (1);
}

/* 
 * Initializes the argument array for the command with the given capacity. 
 * Returns 1 on success, 0 on failure.
 */
int	initialize_arguments(t_command *cmd, size_t *args_capacity)
{
	cmd->args = malloc(sizeof(char *) * *args_capacity);
	return (cmd->args != NULL);
}

/* 
 * Adds an argument to the command's argument array. If the array is full, 
 * it is resized. If memory allocation fails, the program exits with an error.
 */
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
		command->args = ft_realloc(command->args, new_capacity * sizeof(char *));
		if (!command->args)
		{
			ft_putstr_fd("Failed to allocate memory for arguments\n",
				STDERR_FILENO);
			exit(EXIT_FAILURE);
		}
		command->args_capacity = new_capacity;
	}
	command->args[args_count] = ft_strdup(arg);
	command->args[args_count + 1] = NULL;
}
