/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_env.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anilchen <anilchen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/14 17:53:05 by anilchen          #+#    #+#             */
/*   Updated: 2024/12/05 13:22:49 by anilchen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "process.h"

// Executes the `env` command by printing all environment variables.
// - Iterates through the environment linked list (`env_copy`).
// - Only prints variables that have a non-NULL value.
// - Sets the process exit status to success (0).

int	execute_env(t_env *env_copy, t_process *process)
{
	t_env	*cur;
	size_t	key_len;
	size_t	value_len;

	cur = env_copy;
	while (cur != NULL)
	{
		if (ft_strcmp(cur->value, "") != 0)
		{
			key_len = ft_strlen(cur->key);
			value_len = ft_strlen(cur->value);
			write(STDOUT_FILENO, cur->key, key_len);
			write(STDOUT_FILENO, "=", 1);
			write(STDOUT_FILENO, cur->value, value_len);
			write(STDOUT_FILENO, "\n", 1);
		}
		cur = cur->next;
	}
	set_exit_status(process, 0);
	return (EXIT_SUCCESS);
}
