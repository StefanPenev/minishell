/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: spenev <spenev@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/13 13:56:31 by anilchen          #+#    #+#             */
/*   Updated: 2024/11/20 12:32:28 by spenev           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "process.h"
#include "minishell.h"

// stringcompare
int	ft_strcmp(const char *s1, const char *s2)
{
	size_t	i;

	i = 0;
	while (s1[i] != '\0' || s2[i] != '\0')
	{
		if (s1[i] != s2[i])
		{
			return ((unsigned char)s1[i] - (unsigned char)s2[i]);
		}
		i++;
	}
	return (0);
}

// t_process	**add_to_env_array(t_process **env_copy, t_process *new)
// {
// 	int			count;
// 	t_process	**new_env_copy;
// 	printf("hello\n");
// 	count = 0;
// 	while (env_copy && env_copy[count] != NULL)
// 	{
// 		count++;
// 	}
// 	new_env_copy = malloc(sizeof(t_process *) * (count + 2));
// 	if (!new_env_copy)
// 	{
// 		return (NULL);
// 	}
// 	for (int i = 0; i < count; i++)
// 	{
// 		new_env_copy[i] = env_copy[i];
// 	}
// 	new_env_copy[count] = new;
// 	new_env_copy[count + 1] = NULL;
// 	free(env_copy);
// 	printf("new_env_copy created\n");
// 	return (new_env_copy);
// }

// put it to the back
void	minishell_lstadd_back(t_env **lst, t_env *new)
{
	t_env	*current;

	if (lst == NULL || new == NULL)
		return ;
	if (*lst == NULL)
	{
		*lst = new;
		return ;
	}
	current = *lst;
	while (current->next != NULL)
	{
		current = current->next;
	}
	current->next = new;
	// printf("%s hello from adding\n", new->key);
	// printf("%s hello from adding\n", new->value);
}

// create new node
t_env	*minishell_lstnew(void *key, void *value)
{
	t_env	*node;

	node = (t_env *)malloc(sizeof(t_env));
	if (!node)
	{
		return (NULL);
	}
	node->key = key;
	node->value = value;
	node->next = NULL;
	// printf("Debug: new node created\n"); // debug
	// printf("Debug: %s\n", node->key);
	// printf("Debug: %s\n", node->value);
	return (node);
}

int	minishell_lstsize(t_env *lst)
{
	int	count;

	count = 0;
	while (lst != NULL)
	{
		lst = lst->next;
		count++;
	}
	return (count);
}


// stolen from Stefan
// char	*ft_strndup(const char *s, size_t n)
// {
// 	char			*res;
// 	unsigned int	i;

// 	i = 0;
// 	res = malloc(sizeof(char) * (n + 1));
// 	if (res == NULL)
// 		return (NULL);
// 	while (i < n)
// 	{
// 		res[i] = s[i];
// 		i++;
// 	}
// 	res[i] = '\0';
// 	return (res);
// }

char	*ft_strjoin_three(const char *s1, const char *s2, const char *s3)
{
	int len_s1;
	int len_s2;
	int len_s3;
	char *res;

	len_s1 = 0;
	len_s2 = 0;
	len_s3 = 0;
	if (!s1 && !s2 && !s3)
		return (NULL);
	len_s1 = ft_strlen(s1);
	if (!s1)
		len_s1 = 0;
	len_s2 = ft_strlen(s2);
	if (!s2)
		len_s2 = 0;
	len_s3 = ft_strlen(s3);
	if (!s3)
		len_s3 = 0;
	res = (char *)malloc(len_s1 + len_s2 + len_s3 + 1);
	ft_memcpy(res, s1, len_s1);
	ft_memcpy(res + len_s1, s2, len_s2);
	ft_memcpy(res + len_s1 + len_s2, s3, len_s3);
	res[len_s1 + len_s2 + len_s3] = '\0';
	return (res);
}