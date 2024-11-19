/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_exit.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anilchen <anilchen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 13:38:22 by anilchen          #+#    #+#             */
/*   Updated: 2024/11/18 16:22:41 by anilchen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "process.h"

void	minishell_exit(char **splitted, t_env *env_copy, t_process *process)
{
	int	i;
	int	exit_code;

	i = 0;
	// if no arguments exit returns exit_code of last executed command
	if (splitted[1] == NULL)
	{
		exit_code = process->last_exit_status;
		write(1, "exit\n", 5);
		//cleanup(splitted);
		//free_env(&env_copy);
		exit(0);
	}
	// check, if argument is numeric
	if (splitted[1][i] == '-' || splitted[1][i] == '+')
	{
		i++;
	}
	while (splitted[1][i] != '\0')
	{
		if (ft_isdigit(splitted[1][i]) == 0)
		{
            write(2, "minishell: exit: ", 17);
            write(2, splitted[1], ft_strlen(splitted[1]));
            write(2, ": numeric argument required\n", 28);
			//free_env(&env_copy);
			//cleanup(splitted);
			//free_ctrl(ctrl);
			exit(2); // error code
		}
		i++;
	}
	exit_code = ft_atoi(splitted[1]);
	// should be strictly 0 or 1 argumens
	if (splitted[2] != NULL)
	{
        write(2, "minishell: exit: too many arguments\n", 36);
		exit(1);
	}
	// Cast exit_code to 0-255 range
	exit_code = exit_code % 256;
	if (exit_code < 0)
	{
		exit_code += 256;
	}
	write(1, "exit\n", 5);
	//free_env(&env_copy);
	//cleanup(splitted);
	//free_ctrl(ctrl);
	exit(exit_code);
}
