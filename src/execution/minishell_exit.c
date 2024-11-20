/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_exit.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anilchen <anilchen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 13:38:22 by anilchen          #+#    #+#             */
/*   Updated: 2024/11/20 14:47:23 by anilchen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "process.h"

void	exit_msgs(t_command *cmd, t_process *process, int action_flag)
{
	if (action_flag == 0)
	{
		write(1, "exit\n", 5);
		exit(process->last_exit_status);
	}
	if (action_flag == 1)
	{
		write(2, "minishell: exit: too many arguments\n", 36);
		set_exit_status(process, 1);
	}
	else if (action_flag == 2)
	{
		write(1, "exit\n", 5);
		write(2, "minishell: exit: ", 17);
		write(2, cmd->args[1], ft_strlen(cmd->args[1]));
		write(2, ": numeric argument required\n", 28);
		exit(2);
	}
}

int	is_numeric_argument(const char *arg)
{
	int	i;

	i = 0;
	if (arg[0] == '-' || arg[0] == '+')
		i++;
	while (arg[i])
	{
		if (!ft_isdigit(arg[i]))
			return (0);
		i++;
	}
	return (1);
}

int	calculate_exit_code(const char *arg)
{
	int	exit_code;

	exit_code = ft_atoi(arg);
	exit_code = exit_code % 256;
	if (exit_code < 0)
	{
		exit_code += 256;
	}
	return (exit_code);
}

char	*prepare_exit_arg(t_command *cmd)
{
	char	*joined_arg;

	if (cmd->args[1][0] == '-' || cmd->args[1][0] == '+')
	{
		if (cmd->args[2] != NULL)
		{
			joined_arg = ft_strjoin(cmd->args[1], cmd->args[2]);
		}
		else
		{
			joined_arg = ft_strdup(cmd->args[1]);
		}
	}
	else
	{
		joined_arg = ft_strdup(cmd->args[1]);
	}
	return (joined_arg);
}

void	execute_exit(t_command *cmd, t_process *process)
{
	int		exit_code;
	char	*joined_arg;

	joined_arg = NULL;
	if (cmd->args[1] == NULL)
		exit_msgs(cmd, process, 0);
	if (cmd->args[1][0] != '-' && cmd->args[1][0] != '+'
		&& cmd->args[2] != NULL)
	{
		exit_msgs(cmd, process, 1);
		return ;
	}
	joined_arg = prepare_exit_arg(cmd);
	if (!is_numeric_argument(joined_arg))
	{
		free(joined_arg);
		exit_msgs(cmd, process, 2);
	}
	exit_code = calculate_exit_code(joined_arg);
	free(joined_arg);
	write(1, "exit\n", 5);
	exit(exit_code);
}
