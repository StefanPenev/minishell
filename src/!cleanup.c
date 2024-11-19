/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_cleanup.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anilchen <anilchen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/14 13:10:17 by anilchen          #+#    #+#             */
/*   Updated: 2024/11/15 17:23:20 by anilchen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "process.h"

// clean splitted line
void	cleanup(char **splitted)
{
	int	i;

	i = 0;
	while (splitted[i] != NULL)
	{
		free(splitted[i]);
		i++;
	}
	free(splitted);
}

// void	del_t_process(void *content)
// {
// 	t_process *proc = (t_process *)content;
// 	if (proc)
// 	{
// 		free(proc->key);
// 		free(proc->value);
// 		free(proc);
// 	}
// }

// clean linked list
void	free_env(t_env **lst)
{
	t_env	*temp;

	while (*lst)
	{
		temp = (*lst)->next;
		if ((*lst)->key)
			free((*lst)->key);
		if ((*lst)->value)
			free((*lst)->value);
		free(*lst);
		*lst = temp;
	}
	*lst = NULL;
}

//clean control structure
// void	free_ctrl(t_ctrl *ctrl)
// {
// 	if (ctrl->env)
// 		free_env(ctrl->env);
// 	free(ctrl);
// }

// void minishell_lstclear(t_process **node)
// {
//     t_process *temp;

//     while (*node)
//     {
//         temp = (*node)->next;
//         free((*node)->key);
//         free((*node)->value);
//         free(*node);
//         *node = temp;
//     }
//     *node = NULL;
// }

// void	free_env_copy(t_process **env_copy)
// {
// 	int	i;

// 	i = 0;
// 	while (env_copy[i] != NULL)
// 	{
// 		free(env_copy[i]);
// 		i++;
// 	}
// 	free(env_copy);
// }
