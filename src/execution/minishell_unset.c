/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_unset.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anilchen <anilchen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 12:57:45 by anilchen          #+#    #+#             */
/*   Updated: 2024/11/29 15:33:13 by anilchen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "process.h"

// Removes a single node from the linked list with the given key.
// If the node with the specified key is found, it is removed from the list and
// freed. If no such node exists, the function does nothing.

void	del_node(t_env **head, char *key)
{
	t_env	*cur;
	t_env	*prev;

	cur = *head;
	prev = NULL;
	while (cur != NULL)
	{
		if (ft_strcmp(cur->key, key) == 0)
		{
			if (prev == NULL)
				*head = cur->next;
			else
				prev->next = cur->next;
			free(cur->key);
			free(cur->value);
			free(cur);
			return ;
		}
		prev = cur;
		cur = cur->next;
	}
}

// Executes the `unset` command to remove an environment variable.
// If no arguments are provided, it does nothing and exits successfully.
// If the specified variable exists in the list, it is removed.

int	execute_unset(t_command *cmd, t_env *env_copy, t_process *process)
{
	t_env	*cur;
	int		i;

	i = 1;
	cur = env_copy;
	if (cmd->args[1] == NULL)
	{
		set_exit_status(process, 0);
		return (EXIT_SUCCESS);
	}
	while (cmd->args[i] != NULL)
	{
		del_node(&env_copy, cmd->args[i]);
		i++;
	}
	set_exit_status(process, 0);
	return (EXIT_SUCCESS);
}
