/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anilchen <anilchen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/13 13:56:31 by anilchen          #+#    #+#             */
/*   Updated: 2024/11/29 15:46:28 by anilchen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "process.h"

// Compares two strings lexicographically.
// Returns 0 if the strings are equal, a positive value if the first string
// is greater, and a negative value if the second string is greater.

int	ft_strcmp(const char *s1, const char *s2)
{
	size_t	i;

	i = 0;
	if (!s1 || !s2)
	{
		return (0);
	}
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

// Adds a new node to the end of the linked list.
// If the list is empty, the new node becomes the head.

void	node_add_back(t_env **lst, t_env *new)
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
}

// Creates a new linked list node with the given key and value.
// Allocates memory for the node but not for the key or value strings.
// Returns the new node, or NULL if memory allocation fails.

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
	return (node);
}

// Counts the number of nodes in a linked list.
// Traverses the entire list and increments a counter for each node.

int	lstsize(t_env *lst)
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

// Joins three strings into one new string.
// Allocates memory for the result, copies each string into it,
//	and returns the result.
// If all strings are NULL, returns NULL.

char	*ft_strjoin_three(const char *s1, const char *s2, const char *s3)
{
	int		len_s1;
	int		len_s2;
	int		len_s3;
	char	*res;

	len_s1 = 0;
	len_s2 = 0;
	len_s3 = 0;
	if (!s1 && !s2 && !s3)
		return (NULL);
	len_s1 = ft_strlen(s1);
	len_s2 = ft_strlen(s2);
	len_s3 = ft_strlen(s3);
	res = (char *)malloc(len_s1 + len_s2 + len_s3 + 1);
	ft_memcpy(res, s1, len_s1);
	ft_memcpy(res + len_s1, s2, len_s2);
	ft_memcpy(res + len_s1 + len_s2, s3, len_s3);
	res[len_s1 + len_s2 + len_s3] = '\0';
	return (res);
}
