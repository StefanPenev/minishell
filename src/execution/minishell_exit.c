/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_exit.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anilchen <anilchen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 13:38:22 by anilchen          #+#    #+#             */
/*   Updated: 2024/11/29 15:02:07 by anilchen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "process.h"

// Handles the `exit` command output and error messages based on 
// the action_flag:
// - action_flag == 0: Normal exit with the last process exit status.
// - action_flag == 1: Error for too many arguments.
// - action_flag == 2: Error for a non-numeric argument, exits with code 2.

void	exit_msgs(t_command *cmd, t_process *process, int action_flag)
{
	if (action_flag == 0)
	{
		write(1, "exit\n", 5);
		exit(process->last_exit_status);
	}
	if (action_flag == 1)
	{
		write(1, "exit\n", 5);
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

// Checks if the provided argument is a valid numeric string.
// Supports optional '+' or '-' at the start.

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

// Calculates the exit code by taking the numeric value modulo 256.
// Ensures the exit code is always between 0 and 255.
int	calculate_exit_code(const char *arg, t_command *cmd, t_process *process)
{
	long	exit_code;
	int		overflow;

	overflow = 0;
	exit_code = ft_atol(arg, &overflow);
	if (overflow)
	{
		exit_msgs(cmd, process, 2);
	}
	exit_code = exit_code % 256;
	if (exit_code < 0)
	{
		exit_code += 256;
	}
	return ((int)exit_code);
}

// Executes the `exit` command.
// Handles cases for no arguments, too many arguments,
//	and non-numeric arguments.

void	execute_exit(t_command *cmd, t_process *process)
{
	int	exit_code;

	if (cmd->args[1] == NULL)
		exit_msgs(cmd, process, 0);
	if (cmd->args[1][0] != '-' && cmd->args[1][0] != '+'
		&& cmd->args[2] != NULL)
	{
		exit_msgs(cmd, process, 1);
		return ;
	}
	if (!is_numeric_argument(cmd->args[1]))
		exit_msgs(cmd, process, 2);
	exit_code = calculate_exit_code(cmd->args[1], cmd, process);
	write(1, "exit\n", 5);
	exit(exit_code);
}
