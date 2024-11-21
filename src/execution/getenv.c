/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   getenv.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anilchen <anilchen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/14 13:09:36 by anilchen          #+#    #+#             */
/*   Updated: 2024/11/21 14:22:34 by anilchen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "process.h"

// // debug function, delete afterwards
// void	printList(t_env *head)
// {
// 	t_env	*current;

// 	current = head;
// 	while (current != NULL)
// 	{
// 		printf("Key: %s, Value: %s\n", current->key, current->value);
// 		current = current->next;
// 	}
// }

// get anv value
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

// create node
t_env	*create_node(char *env_str)
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

// create linked list
t_env	*init_env(char **envp)
{
	t_env	*env_copy;
	t_env	*new_var;
	int		i;

	env_copy = NULL;
	i = 0;
	while (envp[i] != NULL)
	{
		new_var = create_node(envp[i]);
		minishell_lstadd_back(&env_copy, new_var);
		i++;
	}
	return (env_copy);
}

// set env value
void	my_setenv(char *key, char *value, t_env *env_copy)
{
	t_env	*current;

	current = env_copy;
	while (current != NULL)
	{
		if (ft_strcmp(current->key, key) == 0)
		{
			free(current->value);
			current->value = value;
			return ;
		}
		current = current->next;
	}
	free(value);
}

//creatre env_array from list
char	**create_env_array(t_env *env_copy)
{
	t_env	*cur;
	int		i;
	int		env_count;
	char	**env_array;
	char	*key_value;

	env_count = minishell_lstsize(env_copy);
	// +1 for NULL
	env_array = malloc(sizeof(char *) * (env_count + 1));
	// copying from list to array
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
