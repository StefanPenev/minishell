/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_pwd.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anilchen <anilchen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/14 13:12:27 by anilchen          #+#    #+#             */
/*   Updated: 2024/11/29 15:19:18 by anilchen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "process.h"

// Executes the `pwd` command, which prints the current working directory.
// The command does not require any arguments, but if arguments are provided,
// it still works as expected. It searches for the "PWD" environment variable
// in the linked list and prints its value.
// If "PWD" is not found, the function still exits successfully with no output.

int	execute_pwd(t_env *env_copy, t_process *process)
{
	t_env	*current;

	current = env_copy;
	while (current != NULL)
	{
		if (ft_strcmp(current->key, "PWD") == 0)
		{
			write(1, current->value, ft_strlen(current->value));
			write(1, "\n", 1);
			set_exit_status(process, 0);
			return (EXIT_SUCCESS);
		}
		current = current->next;
	}
	set_exit_status(process, 0);
	return (EXIT_SUCCESS);
}
