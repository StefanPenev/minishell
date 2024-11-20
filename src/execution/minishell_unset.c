/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_unset.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anilchen <anilchen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 12:57:45 by anilchen          #+#    #+#             */
/*   Updated: 2024/11/20 16:52:59 by anilchen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "process.h"

// remove single node
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

// main unset
int	execute_unset(t_command *cmd, t_env *env_copy, t_process *process)
{
	t_env	*cur;

	cur = env_copy;
	if (cmd->args[1] == NULL)
	{
		set_exit_status(process, 0);
		return (EXIT_SUCCESS);
	}
	while (cur != NULL)
	{
		if (ft_strcmp(cur->key, cmd->args[1]) == 0)
		{
			del_node(&env_copy, cur->key);
			break ;
		}
		cur = cur->next;
	}
	set_exit_status(process, 0);
	return (EXIT_SUCCESS);
}
