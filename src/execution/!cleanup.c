/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   !cleanup.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anilchen <anilchen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/14 13:10:17 by anilchen          #+#    #+#             */
/*   Updated: 2024/11/27 14:00:16 by anilchen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "process.h"

// clean splitted line
void	free_splitted(char **splitted)
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

// clean single node
void	free_node(t_env *node)
{
	if (node)
	{
		free(node->key);
		free(node->value);
		free(node);
	}
}

void	cleanup_pipes_process(t_pipes_process_content *ctx)
{
	if (ctx->pid)
	{
		free(ctx->pid);
	}
	if (ctx->env_array)
	{
		free_splitted(ctx->env_array);
	}
}
