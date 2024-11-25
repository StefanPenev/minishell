/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_utils_2.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anilchen <anilchen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/20 16:44:21 by anilchen          #+#    #+#             */
/*   Updated: 2024/11/25 15:18:16 by anilchen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "process.h"

void	swap_nodes(t_env *node1, t_env *node2)
{
	char	*temp_key;
	char	*temp_value;

	temp_key = node1->key;
	temp_value = node1->value;
	node1->key = node2->key;
	node1->value = node2->value;
	node2->key = temp_key;
	node2->value = temp_value;
}

// if file exists and executable
// int	is_executable(const char *path)
// {
// 	if (access(path, X_OK) == 0)
// 	{
// 		return (1);
// 	}
// 	else
// 	{
// 		return (0);
// 	}
// }

int	is_executable(const char *path)
{
	struct stat	sb;

	if (access(path, X_OK) != 0)
	{
		return (0);
	}
	if (stat(path, &sb) != 0)
	{
		return (0);
	}
	if (S_ISREG(sb.st_mode))
	{
		return (1);
	}
	return (0);
}
