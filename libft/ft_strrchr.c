/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: spenev <spenev@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/13 11:35:30 by spenev            #+#    #+#             */
/*   Updated: 2023/12/04 10:35:01 by spenev           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/* 
	DESCRIPTION:
				The strrchr() function is identical to strchr(), except it
				locates the last occurrence of c.

	RETURN VALUES:
				The function strrchr() return a pointer to the located 
				character, or NULL if the character does not appear in the 
				string.
 */

char	*ft_strrchr(const char *s, int c)
{
	char const	*ptr;

	ptr = NULL;
	while (*s)
	{
		if (*s == (char)c)
			ptr = s;
		s++;
	}
	if (*s == (char)c)
		return ((char *)s);
	else
		return ((char *)ptr);
}
