/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_export_2.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anilchen <anilchen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/20 16:39:45 by anilchen          #+#    #+#             */
/*   Updated: 2024/12/05 13:21:14 by anilchen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "process.h"

// Function to sort a linked list of t_env nodes in alphabetical order.
// Uses a bubble sort algorithm by comparing the `key` of each node.

void	sort_t_process_list(t_env **head)
{
	int		swapped;
	t_env	*head_ptr;
	t_env	*last_ptr;

	last_ptr = NULL;
	if (*head == NULL)
		return ;
	swapped = 1;
	while (swapped)
	{
		swapped = 0;
		head_ptr = *head;
		while (head_ptr->next != last_ptr)
		{
			if (head_ptr->key != NULL && head_ptr->next->key != NULL
				&& ft_strcmp(head_ptr->key, head_ptr->next->key) > 0)
			{
				swap_nodes(head_ptr, head_ptr->next);
				swapped = 1;
			}
			head_ptr = head_ptr->next;
		}
		last_ptr = head_ptr;
	}
}

// Function to create a copy of a single node in the t_env list.
// Allocates new memory for the node and duplicates its `key` and `value`.

t_env	*duplicate_t_process_node(t_env *proc)
{
	t_env	*new_proc;

	if (proc == NULL)
		return (NULL);
	new_proc = malloc(sizeof(t_env));
	if (!new_proc)
		return (NULL);
	new_proc->key = ft_strdup(proc->key);
	if (proc->value != NULL)
		new_proc->value = ft_strdup(proc->value);
	else
		new_proc->value = NULL;
	if (!new_proc->key || (proc->value && !new_proc->value))
	{
		free_node(new_proc);
		return (NULL);
	}
	new_proc->next = NULL;
	return (new_proc);
}

// Function to create a full copy of a t_env linked list.
// Allocates new nodes and duplicates the content of the original list.

t_env	*duplicate_t_process_list(t_env *list)
{
	t_env	*new_list;
	t_env	*new_node;
	t_env	*last;

	new_list = NULL;
	last = NULL;
	while (list != NULL)
	{
		new_node = duplicate_t_process_node(list);
		if (new_list == NULL)
			new_list = new_node;
		else
			last->next = new_node;
		last = new_node;
		list = list->next;
	}
	return (new_list);
}

// Function to handle the `export` command with arguments (key, value).
// If the variable exists, updates its value; otherwise, creates a new node and
// adds it to the list.

int	export_no_args(t_env *env_copy)
{
	t_env	*sorted_list;
	t_env	*current;

	sorted_list = NULL;
	sorted_list = duplicate_t_process_list(env_copy);
	sort_t_process_list(&sorted_list);
	current = sorted_list;
	while (current != NULL)
	{
		if (current->key != NULL && ft_strcmp(current->value, "") != 0)
			printf("declare -x %s=\"%s\"\n", current->key, current->value);
		else if (current->key != NULL)
			printf("declare -x %s\n", current->key);
		current = current->next;
	}
	free_env(&sorted_list);
	return (EXIT_SUCCESS);
}

// if we have found variable in the list - replase value, else - create new node
int	export_with_args(char *key, char *value, t_env *env_copy)
{
	t_env	*current;
	int		found;
	t_env	*new_var;

	found = 0;
	current = env_copy;
	while (current != NULL)
	{
		if (ft_strcmp(key, current->key) == 0)
		{
			my_setenv(key, value, env_copy);
			found = 1;
			break ;
		}
		current = current->next;
	}
	if (!found)
	{
		new_var = minishell_lstnew(key, value);
		node_add_back(&env_copy, new_var);
	}
	return (EXIT_SUCCESS);
}
