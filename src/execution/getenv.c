/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   getenv.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: spenev <spenev@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/14 13:09:36 by anilchen          #+#    #+#             */
/*   Updated: 2024/11/20 11:12:02 by spenev           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "process.h"
#include "minishell.h"

// t_process	*create_line(char *env_str)
// {
// 	t_process	*sring;
// 	char		*equal;

// 	sring = malloc(sizeof(t_process));
// 	equal = ft_strchr(env_str, '=');
// 	if (!equal)
// 	{
// 		sring->key = ft_strdup(env_str);
// 		sring->value = NULL;
// 	}
// 	else
// 	{
// 		*equal = '\0';
// 		sring->key = ft_strdup(env_str);
// 		sring->value = ft_strdup(equal + 1);
// 		*equal = '=';
// 	}
// 	return (sring);
// }

// debug function
void	printList(t_env *head)
{
	t_env	*current;

	current = head;
	while (current != NULL)
	{
		printf("Key: %s, Value: %s\n", current->key, current->value);
		current = current->next;
	}
}

//create node
t_env	*create_node(char *env_str)
{
	t_env	*node;
	char		*equal;

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

//create linked list
t_env	*init_env(char **envp)
{
	t_env	*env_copy;
	t_env	*new_var;
	int			i;

	env_copy = NULL;
	i = 0;
	while (envp[i] != NULL)
	{
		// minishell_lstnew(void *key, void *value);
		new_var = create_node(envp[i]);
		minishell_lstadd_back(&env_copy, new_var);
		i++;
	}
	//printList(env_copy);
	return (env_copy);
}

// t_process	**init_env(char **envp)
// {
// 	t_process	**env_copy;
// 	int			i;

// 	i = 0;
// 	while (envp[i] != NULL)
// 	{
// 		i++;
// 	}
// 	env_copy = malloc(sizeof(t_process *) * (i + 1));
// 	i = 0;
// 	while (envp[i] != NULL)
// 	{
// 		env_copy[i] = create_line(envp[i]);

// 		i++;
// 	}
// 	env_copy[i] = NULL;
// 	return (env_copy);
// }

void	my_setenv(char *key, char *value, t_env *env_copy)
{
	t_env	*current;

	current = env_copy;
	while (current != NULL)
	{
		if (strcmp(current->key, key) == 0)
		{
			free(current->value);
			current->value = value;
			return ;
		}
		current = current->next;
	}
	free(value);
}
