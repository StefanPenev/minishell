/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_env.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anilchen <anilchen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/14 17:53:05 by anilchen          #+#    #+#             */
/*   Updated: 2024/11/26 12:35:31 by anilchen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "process.h"

int	execute_env(t_env *env_copy, t_process *process)
{
	t_env	*cur;

	cur = env_copy;
	while (cur != NULL)
	{
		if (cur->value == NULL)
		{
			cur = cur->next;
		}
		else
		{
			printf("%s=%s\n", cur->key, cur->value);
			cur = cur->next;
		}
	}
	set_exit_status(process, 0);
	return (EXIT_SUCCESS);
}
