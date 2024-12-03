/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   !cleanup.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anilchen <anilchen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/14 13:10:17 by anilchen          #+#    #+#             */
/*   Updated: 2024/12/03 17:01:19 by anilchen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "process.h"

// Frees memory allocated for a null-terminated array of strings.
// Iterates through the array, freeing each string,
//	and then frees the array itself.
// Parameters:
//   - splitted: A pointer to the null-terminated array of strings to be freed.
// Notes:
//	- This function is typically used for freeing arrays created by string
// splitting functions.
//	- Handles null-terminated arrays gracefully to ensure all elements
// are freed.

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

// Frees memory allocated for a linked list of environment variables.
// Iterates through the list, freeing the `key`, `value`,
//	and node itself for each element.
// Sets the pointer to the list head to NULL after freeing all nodes.
// Parameters:
//   - lst: A double pointer to the head of the linked list to be freed.
// Notes:
//   - Ensures no dangling pointers by setting the head pointer to NULL.
//   - Handles cases where the list is already empty (`*lst == NULL`).

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

// Frees memory allocated for a single environment variable node.
// Frees the `key`, `value`, and the node itself.
// Parameters:
//   - node: A pointer to the node to be freed.
// Notes:
//   - Handles cases where the node or its members (`key` or `value`) are NULL.

void	free_node(t_env *node)
{
	if (node)
	{
		free(node->key);
		free(node->value);
		free(node);
	}
}

// Frees memory allocated for resources in a pipeline process context.
// Frees the array of process IDs and the environment variable array.
// Parameters:
// - ctx: A pointer to the pipeline process context structure to be cleaned up.
// Notes:
//   - Ensures no dangling pointers by safely freeing dynamic allocations.
//- Handles cases where the context members (`pid` or `env_array`) are
// already NULL.

void	cleanup_pipes(t_pipes_process_content *ctx)
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
