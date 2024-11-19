/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_env.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anilchen <anilchen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/14 17:53:05 by anilchen          #+#    #+#             */
/*   Updated: 2024/11/18 17:19:41 by anilchen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "process.h"

// int	minishell_env(t_process **env_copy)
// {
// 	int i = 0;
// 	while (env_copy[i] != NULL)
// 	{
// 		printf("%s=%s\n", env_copy[i]->key, env_copy[i]->value);
// 		i++;
// 	}
// 	return (EXIT_SUCCESS);
// }

int	minishell_env(char **args, t_env *env_copy, t_process *process)
{
	t_env	*cur;

	cur = env_copy;
	while (cur != NULL)
	{
		printf("%s=%s\n", cur->key, cur->value);
		cur = cur->next;
	}
	set_exit_status(process, 0);
	return (EXIT_SUCCESS);
}
