/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_export.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anilchen <anilchen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/14 13:14:41 by anilchen          #+#    #+#             */
/*   Updated: 2024/11/21 12:37:19 by anilchen         ###   ########.fr       */
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

void	split_key_value(char *arg, char **key, char **value)
{
	char	*equal_sign_pos;
	size_t	key_length;

	equal_sign_pos = ft_strchr(arg, '=');
	if (equal_sign_pos != NULL)
	{
		key_length = equal_sign_pos - arg;
		*key = ft_strndup(arg, key_length);
		*value = ft_strdup(equal_sign_pos + 1);
	}
	else
	{
		*key = ft_strdup(arg);
		*value = NULL;
	}
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

	i = 1;
	if (cmd->args[1] == NULL)
	{
		export_no_args(env_copy);
		set_exit_status(process, 0);
		return (EXIT_SUCCESS);
	}
	while (cmd->args[i] != NULL)
	{
		split_key_value(cmd->args[i], &key, &value);
		if (process_export_argument(key, value, env_copy,
				process) == EXIT_FAILURE)
			return (EXIT_FAILURE);
		i++;
	}
	set_exit_status(process, 0);
	return (EXIT_SUCCESS);
}
