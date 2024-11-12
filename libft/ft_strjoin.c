/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: spenev <spenev@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/13 13:01:38 by spenev            #+#    #+#             */
/*   Updated: 2023/11/14 11:28:24 by spenev           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* 
	DESCRIPTION:
				Allocates (with malloc(3)) and returns a new string, which is 
				the result of the concatenation of ’s1’ and ’s2’.

	RETURN VALUES:
				The new string. NULL if the allocation fails.
 */

#include "libft.h"

char	*ft_strjoin(char const *s1, char const *s2)
{
	int		length;
	char	*str;
	char	*result_str;

	if (!s1 || !s2)
		return (NULL);
	length = ft_strlen(s1) + ft_strlen(s2) + 1;
	str = (char *)malloc(length * sizeof(char));
	if (!str)
		return (NULL);
	result_str = str;
	while (*s1)
		*str++ = *s1++;
	while (*s2)
		*str++ = *s2++;
	*str++ = '\0';
	return (result_str);
}
