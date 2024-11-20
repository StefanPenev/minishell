/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_pwd.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anilchen <anilchen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/14 13:12:27 by anilchen          #+#    #+#             */
/*   Updated: 2024/11/20 13:40:53 by anilchen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "process.h"

// pwd shouldnt have any arguments, but if they are, it still works
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
