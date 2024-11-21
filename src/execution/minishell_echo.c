/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_echo.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anilchen <anilchen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/14 13:08:31 by anilchen          #+#    #+#             */
/*   Updated: 2024/11/21 14:45:51 by anilchen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "process.h"

int	execute_echo(t_command *cmd, t_process *process)
{
	bool	no_newline;
	int		i;

	// char	*exit_status;
	printf("Debug(echo): exit status: %d\n", process->last_exit_status);
	no_newline = false;
	i = 1;
	while (cmd->args[i] && ft_strcmp(cmd->args[i], "-n") == 0)
	{
		no_newline = true;
		i++;
	}
	while (cmd->args[i] != NULL)
	{
		// exit_status = ft_itoa(process->last_exit_status);
		// if (ft_strcmp(cmd->args[i], exit_status) == 0 && cmd->args[i
		// 	+ 1] != NULL)
		// {
		// 	write(1, cmd->args[i], ft_strlen(cmd->args[i]));
		// 	write(1, cmd->args[i + 1], ft_strlen(cmd->args[i + 1]));
		// 	i += 2;
		// }
		// else
		// {
		write(1, cmd->args[i], ft_strlen(cmd->args[i]));
		i++;
		// }
		if (cmd->args[i] != NULL)
			write(1, " ", 1);
	}
	// free(exit_status);
	if (!no_newline)
		write(1, "\n", 1);
	set_exit_status(process, 0);
	return (EXIT_SUCCESS);
}

// void	expand_and_print(char *var, t_env *env_copy, t_process *process)
// {
// 	int		i;
// 	int		len;
// 	int		start;
// 	int		name_len;
// 	char	*name;
// 	char	*value;

// 	i = 0;
// 	len = ft_strlen(var);
// 	if (len < 2)
// 	{
// 		write(1, "$", 1);
// 		return ;
// 	}
// 	while (i < len)
// 	{
// 		if (var[i] == '$')
// 		{
// 			i++; // skip '$'
// 			if (i < len)
// 			{
// 				// case for $?
// 				if (var[i] == '?')
// 				{
// 					value = get_env_value("?", env_copy, process);
// 					write(1, value, ft_strlen(value));
// 					free(value);
// 					i++;
// 				}
// 				// if variable valid(shouldnt be digit or symbol)
// 				else if (ft_isalpha(var[i]) || var[i] == '_')
// 				{
// 					start = i;
// 					while (i < len && (ft_isalnum(var[i]) || var[i] == '_'))
// 					{
// 						i++;
// 					}
// 					name_len = i - start;
// 					name = ft_strndup(var + start, name_len);
// 					value = get_env_value(name, env_copy, process);
// 					write(1, value, ft_strlen(value));
// 					free(name);
// 					free(value);
// 				}
// 				else
// 				{
// 					// no valid name after '$',
// 					//	выводим '$' как есть
// 					write(1, "$", 1);
// 					write(1, &var[i], 1);
// 					i++;
// 				}
// 			}
// 			else
// 			{
// 				// just '$'
// 				write(1, "$", 1);
// 			}
// 		}
// 		else
// 		{
// 			// simple prompt
// 			write(1, &var[i], 1);
// 			i++;
// 		}
// 	}
// }

// int	echo_env_var(char *var, t_env *env_copy, t_process *process)
// {
// 	if (!var)
// 	{
// 		return (0);
// 	}
// 	expand_and_print(var, env_copy, process);
// 	return (1);
// }

// int	execute_echo(t_command *cmd, t_process *process)
// {
// 	bool	no_newline;
// 	int		i;

// 	i = 1;
// 	no_newline = false;
// 	while (cmd->args[i] && ft_strcmp(cmd->args[i], "-n") == 0)
// 	{
// 		no_newline = true;
// 		i++;
// 	}
// 	// main loop to work with multiply arguments
// 	while (cmd->args[i] != NULL)
// 	{
// 		// if env_variable
// 		// if (splitted[i][0] == '$')
// 		// {
// 		// 	if (!echo_env_var(splitted[i], env_copy, process))
// 		// 	{
// 		// 		// if env_variable doesnt exist skip argument
// 		// 		i++;
// 		// 		continue ;
// 		// 	}
// 		// }
// 		// if simple text
// 		// else
// 		// {
// 		// just print
// 		write(1, cmd->args[i], ft_strlen(cmd->args[i]));
// 		//}
// 		// add space if argument is not last
// 		if (cmd->args[i + 1])
// 			write(1, " ", 1);
// 		i++;
// 	}
// 	// if no_newline = false add newline
// 	if (!no_newline)
// 	{
// 		write(1, "\n", 1);
// 	}
// 	set_exit_status(process, 0);
// 	return (EXIT_SUCCESS);
// }

// int	execute_echo(t_command *cmd, t_process *process)
// {
// 	bool	no_newline;
// 	int		i;
// 	char	*arg;

// 	i = 1;
// 	no_newline = false;
// 	// Проверяем на наличие флага "-n"
// 	if (cmd->args[1] && ft_strcmp(cmd->args[1], "-n") == 0)
// 	{
// 		no_newline = true;
// 		i++;
// 	}
// 	// Основной цикл для обработки аргументов
// 	while (cmd->args[i] != NULL)
// 	{
// 		arg = cmd->args[i];
// 		// Если аргумент содержит переменную окружения (например, $?)
// 		if (arg[0] == '$' && arg[1] == '?')
// 		{
// 			// Подставляем статус выхода
// 			ft_putnbr_fd(process->last_exit_status, 1);
// 			// Если есть текст после переменной, печатаем его
// 			write(1, arg + 2, ft_strlen(arg + 2));
// 		}
// 		else
// 		{
// 			// Просто печатаем текст
// 			write(1, arg, ft_strlen(arg));
// 		}
// 		// Добавляем пробел, если аргумент не последний
// 		if (cmd->args[i + 1])
// 			write(1, " ", 1);
// 		i++;
// 	}
// 	// Если флаг "-n" не указан, добавляем перевод строки
// 	if (!no_newline)
// 	{
// 		write(1, "\n", 1);
// 	}
// 	set_exit_status(process, 0);
// 	return (EXIT_SUCCESS);
// }
