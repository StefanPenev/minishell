/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_echo.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anilchen <anilchen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/14 13:08:31 by anilchen          #+#    #+#             */
/*   Updated: 2024/11/20 17:03:15 by anilchen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "process.h"

// function to get env value or `$?`
char	*get_env_value(char *name, t_env *env_copy, t_process *process)
{
	t_env	*cur;
	char	*exit_status;

	if (ft_strcmp(name, "?") == 0)
	{
		exit_status = ft_itoa(process->last_exit_status);
		return (exit_status);
	}
	cur = env_copy;
	while (cur != NULL)
	{
		if (ft_strcmp(cur->key, name) == 0)
		{
			return (ft_strdup(cur->value));
		}
		cur = cur->next;
	}
	// if no variable, return empty string
	return (ft_strdup(""));
}

void	expand_and_print(char *var, t_env *env_copy, t_process *process)
{
	int		i;
	int		len;
	int		start;
	int		name_len;
	char	*name;
	char	*value;

	i = 0;
	len = ft_strlen(var);
	if (len < 2)
	{
		write(1, "$", 1);
		return ;
	}
	while (i < len)
	{
		if (var[i] == '$')
		{
			i++; // skip '$'
			if (i < len)
			{
				//case for $?
				if (var[i] == '?')
				{
					value = get_env_value("?", env_copy, process);
					write(1, value, ft_strlen(value));
					free(value);
					i++;
				}
				// if variable valid(shouldnt be digit or symbol)
				else if (ft_isalpha(var[i]) || var[i] == '_')
				{
					start = i;
					while (i < len && (ft_isalnum(var[i]) || var[i] == '_'))
					{
						i++;
					}
					name_len = i - start;
					name = ft_strndup(var + start, name_len);
					value = get_env_value(name, env_copy, process);
					write(1, value, ft_strlen(value));
					free(name);
					free(value);
				}
				else
				{
					// no valid name after '$',
					//	выводим '$' как есть
					write(1, "$", 1);
					write(1, &var[i], 1);
					i++;
				}
			}
			else
			{
				// just '$'
				write(1, "$", 1);
			}
		}
		else
		{
			// simple prompt
			write(1, &var[i], 1);
			i++;
		}
	}
}

int	echo_env_var(char *var, t_env *env_copy, t_process *process)
{
	if (!var)
	{
		return (0);
	}
	expand_and_print(var, env_copy, process);
	return (1);
}

int	execute_echo(t_command *cmd, t_env *env_copy, t_process *process)
{
	bool	no_newline;

	int i = 1;
	no_newline = false;
	if (cmd->args[1] && ft_strcmp(cmd->args[1], "-n") == 0)
	{
		no_newline = true;
		i++;
	}
	// main loop to work with multiply arguments
	while (cmd->args[i])
	{
		// if env_variable
		if (splitted[i][0] == '$')
		{
			if (!echo_env_var(splitted[i], env_copy, process))
			{
				// if env_variable doesnt exist skip argument
				i++;
				continue ;
			}
		}
		// if simple text
		else
		{
			// just print
			write(1, splitted[i], ft_strlen(splitted[i]));
		}
		// add space if argument is not last
		if (splitted[i + 1])
			write(1, " ", 1);
		i++;
	}
	// if no_newline = false add newline
	if (!no_newline)
	{
		write(1, "\n", 1);
	}
	set_exit_status(process, 0);
	return (EXIT_SUCCESS);
}
