/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stefan <stefan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/13 11:35:15 by spenev            #+#    #+#             */
/*   Updated: 2024/10/22 23:34:01 by stefan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/* 
	DESCRIPTION:
				The strdup() function allocates sufficient memory for a copy 
                of the string s1, does the copy, and returns a pointer to it.
                The pointer may subsequently be used as an argument to the 
                function free(3).

	RETURN VALUES:
				The function strchr() return a pointer to the located character, 
				or NULL if the character does not appear in the string.
 */

char	*ft_strdup(const char *s1)
{
	char	*dup;
	size_t	len;

	len = ft_strlen(s1) + 1;
	dup = (char *)malloc(sizeof(char) * len);
	if (!dup)
		return (NULL);
	ft_memcpy(dup, s1, len);
	return (dup);
}
