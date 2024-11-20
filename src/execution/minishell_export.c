/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_export.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anilchen <anilchen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/14 13:14:41 by anilchen          #+#    #+#             */
/*   Updated: 2024/11/20 16:41:22 by anilchen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "process.h"

int	is_valid(char *arg)
{
	int	i;

	if (!arg || arg[0] == '\0')
		return (0);
	if (!((arg[0] >= 'A' && arg[0] <= 'Z') || (arg[0] >= 'a' && arg[0] <= 'z')
			|| arg[0] == '_'))
		return (0);
	i = 1;
	while (arg[i] != '\0')
	{
		if (!((arg[i] >= 'A' && arg[i] <= 'Z') || (arg[i] >= 'a'
					&& arg[i] <= 'z') || (arg[i] >= '0' && arg[i] <= '9')
				|| arg[i] == '_'))
		{
			return (0);
		}
		i++;
	}
	return (1);
}

char	*handle_lexer(t_command *cmd, int *i)
{
	char	*joined_arg;

	if (cmd->args[*i][ft_strlen(cmd->args[*i]) - 1] == '=')
	{
		if (cmd->args[*i + 1] != NULL)
		{
			joined_arg = ft_strjoin(cmd->args[*i], cmd->args[*i + 1]);
			(*i)++; // Skip the next token
		}
		else
			joined_arg = ft_strdup(cmd->args[*i]);
	}
	else
		joined_arg = ft_strdup(cmd->args[*i]);
	return (joined_arg);
}

void	split_key_value(char *joined_arg, char **key, char **value)
{
	char	**var_to_add;

	var_to_add = ft_split(joined_arg, '=');
	*key = ft_strdup(var_to_add[0]);
	if (var_to_add[1] != NULL)
		*value = ft_strdup(var_to_add[1]);
	else
		*value = NULL;
	free_splitted(var_to_add);
}

int	process_export_argument(char *key, char *value, t_env *env_copy,
		t_process *process)
{
	if (!is_valid(key))
	{
		write(2, "minishell: export: ", 19);
		write(2, key, ft_strlen(key));
		write(2, ": not a valid identifier\n", 25);
		set_exit_status(process, 1);
		return (EXIT_FAILURE);
	}
	export_with_args(key, value, env_copy);
	return (EXIT_SUCCESS);
}

// main export
int	execute_export(t_command *cmd, t_env *env_copy, t_process *process)
{
	int		i;
	char	*key;
	char	*value;
	char	*joined_arg;

	i = 1;
	if (cmd->args[1] == NULL)
	{
		export_no_args(env_copy);
		set_exit_status(process, 0);
		return (EXIT_SUCCESS);
	}
	while (cmd->args[i] != NULL)
	{
		joined_arg = handle_lexer(cmd, &i);
		split_key_value(joined_arg, &key, &value);
		free(joined_arg);
		if (process_export_argument(key, value, env_copy,
				process) == EXIT_FAILURE)
			return (EXIT_FAILURE);
		i++;
	}
	set_exit_status(process, 0);
	return (EXIT_SUCCESS);
}
