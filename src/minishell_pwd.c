/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_pwd.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anilchen <anilchen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/14 13:12:27 by anilchen          #+#    #+#             */
/*   Updated: 2024/11/15 16:32:09 by anilchen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "process.h"

int	minishell_pwd(char **splitted, t_env *env_copy, t_process *process)
{
	t_env	*current;

	current = env_copy;
	// pwd shouldnt have any arguments, but if they are, it still works
	while (current != NULL)
	{
		// printf("Key: %s, Value: %s\n", current->key, current->value);
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

// int	minishell_pwd(char **splitted, t_process **env_copy)
// {
// 	int	i;

// 	i = 0;
// 	if (splitted[1] != NULL)
// 	{
// 		printf("minishell: pwd: too many arguments\n");
// 		return (EXIT_FAILURE);
// 	}
// 	while (env_copy[i] != NULL)
// 	{
// 		if (ft_strcmp(env_copy[i]->key, "PWD") == 0)
// 		{
// 			write(1, env_copy[i]->value, ft_strlen(env_copy[i]->value));
// 			write(1, "\n", 1);
// 			return (EXIT_SUCCESS);
// 		}
// 		i++;
// 	}
// 	return (EXIT_SUCCESS);
// }
