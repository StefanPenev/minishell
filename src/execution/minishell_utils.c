/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anilchen <anilchen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/13 13:56:31 by anilchen          #+#    #+#             */
/*   Updated: 2024/11/21 13:51:07 by anilchen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "process.h"

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
	return (node);
}

//find size of linked list
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

//join 3 strings together
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
