/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_echo.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anilchen <anilchen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/14 13:08:31 by anilchen          #+#    #+#             */
/*   Updated: 2024/11/19 12:29:56 by anilchen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "process.h"

// int	echo_env_var(char *var, t_env *env_copy, t_process *process)
// {
// 	int		len;
// 	char	*cleaned;
// 	t_env	*current;
// 	int		i;
// 	int		vars_count;
// 	char	**recognised_commands;
// 	int		i;

// 	vars_count = 0;
// 	current = env_copy;
// 	len = ft_strlen(var);
// 	// if jist $ and nothing more
// 	if (len < 2)
// 	{
// 		write(1, "$", 1);
// 		return (0);
// 	}
// 	i = 0;
// 	while (i < len)
// 	{
// 		if (var[i] == '$')
// 		{
// 			vars_count++;
// 		}
// 		i++;
// 	}
// 	if (vars_count == 1)
// 	{
// 		// ignore $
// 		cleaned = ft_strndup(var + 1, len - 1);
// 		// if $?
// 		i = 1;
// 		if (ft_strncmp(cleaned, "?", 1) == 0)
// 		{
// 			call_exit_status(process);
// 			// if $?HELLO
// 			while (cleaned[i] != '\0')
// 			{
// 				ft_putchar_fd(cleaned[i], 1);
// 				i++;
// 			}
// 			free(cleaned);
// 			return (1);
// 		}
// 		while (current != NULL)
// 		{ // find variable
// 			if (ft_strcmp(current->key, cleaned) == 0)
// 			{
// 				write(1, current->value, ft_strlen(current->value));
// 				free(cleaned);
// 				return (1);
// 			}
// 			current = current->next;
// 		}
// 		free(cleaned);
// 		return (0);
// 	}
// 	else
// 	{
// 		recognised_commands = ft_split(var, "$");
// 		// Debug
// 		for (int i = 0; i < vars_count; i++)
// 		{
// 			printf("Debug: %s/n", recognised_commands[i]);
// 		}
// 		// end
// 		i = 0;
// 		while (i < vars_count)
// 		{
// 			if (ft_strncmp(cleaned, "?", 1) == 0)
// 			{
// 				call_exit_status(process);
// 				// if $?HELLO
// 				while (cleaned[i] != '\0')
// 				{
// 					ft_putchar_fd(cleaned[i], 1);
// 					i++;
// 				}
// 				return (1);
// 			}
// 			current = env_copy;
// 			while (current != NULL)
// 			{
// 				if (ft_strcmp(current->key, recognised_commands[i]) == 0)
// 				{
// 					write(1, current->value, ft_strlen(current->value));
// 				}
// 				current = current->next;
// 			}
// 			i++;
// 		}
// 	}
// }

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

int	minishell_echo(char **splitted, t_env *env_copy, t_process *process)
{
	bool	no_newline;

	int i = 1; // splitted[0] is "echo"
	no_newline = false;
	// check for "-n"
	if (splitted[1] && ft_strcmp(splitted[1], "-n") == 0)
	{
		no_newline = true;
		i++;
	}
	// main loop to work with multiply arguments
	while (splitted[i])
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
