/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_export.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anilchen <anilchen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/14 13:14:41 by anilchen          #+#    #+#             */
/*   Updated: 2024/12/05 15:02:57 by anilchen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "process.h"

// Validates if the given string `arg` is a valid identifier.
// An identifier must start with a letter or underscore (_) and can contain
// only letters, digits, or underscores.

int	is_valid(char *arg)
{
	int	i;

	i = 1;
	if (!arg || arg[0] == '\0')
		return (0);
	if (!ft_isalpha(arg[0]) && arg[0] != '_')
		return (0);
	while (arg[i] != '\0')
	{
		if (!ft_isalnum(arg[i]) && arg[i] != '_')
			return (0);
		i++;
	}
	return (1);
}

// Splits the argument `arg` into a key and a value based on the '=' character.
// If '=' is found, `key` will be the part before '=' and `value` will be the
// part after it. If '=' is not found, the entire `arg` becomes the key,
//	and `value` is set to NULL.

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
		*value = ft_strdup("");
	}
}

// Processes the given `key` and `value` for the export command.
// If the `key` is invalid, prints an error and sets the process exit status.
// Otherwise, adds or updates the variable in the environment.

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

// Main function to handle the `export` command. If no arguments are
// provided, calls `export_no_args` to list environment variables.
// Otherwise, processes each argument to add or update variables.

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
