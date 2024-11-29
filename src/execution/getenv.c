/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   getenv.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anilchen <anilchen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/14 13:09:36 by anilchen          #+#    #+#             */
/*   Updated: 2024/11/29 17:11:31 by anilchen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "process.h"

// This function retrieves the value of an environment variable from
// the linked list.
// If the variable name is "?", it returns the last exit status as a string.
// If the variable is not found, it returns an empty string.

char	*get_env_value(char *name, t_env *env_copy, t_process *process)
{
	t_env	*cur;
	char	*exit_status;

	if (ft_strcmp(name, "?") == 0)
	{
		exit_status = ft_itoa(process->last_exit_status);
		return (exit_status);
	}
	cur = env_copy;
	while (cur != NULL)
	{
		if (ft_strcmp(cur->key, name) == 0)
		{
			return (ft_strdup(cur->value));
		}
		cur = cur->next;
	}
	return (ft_strdup(""));
}

// This function creates a new node for the environment linked list.
// It takes a key=value string, separates them, and stores them in the node.
// If there is no '=', it sets the value to NULL.

t_env	*create_env_node(char *env_str)
{
	t_env	*node;
	char	*equal;

	node = malloc(sizeof(t_env));
	if (!node)
		return (NULL);
	equal = ft_strchr(env_str, '=');
	if (!equal)
	{
		node->key = ft_strdup(env_str);
		node->value = NULL;
	}
	else
	{
		*equal = '\0';
		node->key = ft_strdup(env_str);
		node->value = ft_strdup(equal + 1);
		*equal = '=';
	}
	node->next = NULL;
	return (node);
}

// This function initializes the environment linked list from the envp array.
// It iterates over envp, creates a node for each entry,
//	and appends it to the list.

t_env	*init_env(char **envp)
{
	t_env	*env_copy;
	t_env	*new_var;
	int		i;

	env_copy = NULL;
	i = 0;
	while (envp[i] != NULL)
	{
		new_var = create_env_node(envp[i]);
		node_add_back(&env_copy, new_var);
		i++;
	}
	return (env_copy);
}

// This function updates or adds an environment variable in the linked list.
// If the key exists, it updates its value. If not,
// it frees the value without adding.

void	my_setenv(char *key, char *value, t_env *env_copy)
{
	t_env	*current;

	current = env_copy;
	while (current != NULL)
	{
		if (ft_strcmp(current->key, key) == 0)
		{
			free(current->value);
			current->value = ft_strdup(value);
			return ;
		}
		current = current->next;
	}
	free(value);
}

// This function converts the environment linked list into a
// NULL-terminated array. Each entry is formatted as key=value.
// It allocates memory dynamically for the array.

char	**create_env_array(t_env *env_copy)
{
	t_env	*cur;
	int		i;
	int		env_count;
	char	**env_array;
	char	*key_value;

	env_count = lstsize(env_copy);
	env_array = malloc(sizeof(char *) * (env_count + 1));
	cur = env_copy;
	i = 0;
	while (cur != NULL)
	{
		key_value = ft_strjoin_three(cur->key, "=", cur->value);
		if (!key_value)
		{
			free_splitted(env_array);
			return (NULL);
		}
		env_array[i] = key_value;
		cur = cur->next;
		i++;
	}
	env_array[i] = NULL;
	return (env_array);
}
