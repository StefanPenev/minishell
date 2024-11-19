/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_unset.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anilchen <anilchen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 12:57:45 by anilchen          #+#    #+#             */
/*   Updated: 2024/11/15 18:07:11 by anilchen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "process.h"

// remove single node
void	ft_minishell_lstdelone(t_env **head, char *key)
{
	t_env	*cur;
	t_env	*prev;
    //current node
	cur = *head;
    //previous node
	prev = NULL;
	while (cur != NULL)
	{
		if (ft_strcmp(cur->key, key) == 0)
		{
			// if node is first
			if (prev == NULL)
			{
				*head = cur->next;
			}
			// if not
			else
			{
                //exclude current node from chain
				prev->next = cur->next;
			}
			free(cur->key);
			free(cur->value);
			free(cur);
			return;
		}
        // update prev and cur
		prev = cur;
		cur = cur->next;
	}
}

// main unset
int	minishell_unset(char **splitted, t_env *env_copy, t_process *process)
{
	t_env *cur;

	cur = env_copy;
	// unset without arguments
	if (splitted[1] == NULL)
	{
		set_exit_status(process, 0);
		return (EXIT_SUCCESS);
	}
	// looking for argument
	while (cur != NULL)
	{
		if (ft_strcmp(cur->key, splitted[1]) == 0)
		{
			ft_minishell_lstdelone(&env_copy, cur->key);
			break;
		}
        cur = cur->next;
	}	
	set_exit_status(process, 0);
    return (EXIT_SUCCESS);

}