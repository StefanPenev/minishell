/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_echo.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anilchen <anilchen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/14 13:08:31 by anilchen          #+#    #+#             */
/*   Updated: 2024/12/03 16:48:48 by anilchen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "process.h"

// Executes the `echo` command with support for the `-n` flag to
// suppress the trailing newline.
// Arguments are printed to the standard output, separated by spaces.
// - `cmd->args[i]`: Holds the arguments passed to the `echo` command.
// - `-n`: If present, suppresses the trailing newline.
// - `set_exit_status`: Sets the exit status to 0 (success) after execution.

int	execute_echo(t_command *cmd, t_process *process)
{
	bool	no_newline;
	int		i;

	no_newline = false;
	i = 1;
	while (cmd->args[i] && ft_strcmp(cmd->args[i], "-n") == 0)
	{
		no_newline = true;
		i++;
	}
	while (cmd->args[i] != NULL)
	{
		write(STDOUT_FILENO, cmd->args[i], ft_strlen(cmd->args[i]));
		i++;
		if (cmd->args[i] != NULL)
			write(STDOUT_FILENO, " ", 1);
	}
	if (!no_newline)
		write(STDOUT_FILENO, "\n", 1);
	set_exit_status(process, 0);
	return (EXIT_SUCCESS);
}
