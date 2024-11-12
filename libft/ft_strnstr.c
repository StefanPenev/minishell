/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: spenev <spenev@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/13 11:35:27 by spenev            #+#    #+#             */
/*   Updated: 2023/12/04 10:34:40 by spenev           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/* 
	DESCRIPTION:
				The strnstr() function locates the first occurrence of the 
				null-terminated string needle in the string haystack, where not
				more than len characters are searched.  Characters that appear 
				after a ‘\0’ character are not searched.

	RETURN VALUES:
				If needle is an empty string, haystack is returned; if needle 
				occurs nowhere in haystack, NULL is returned; otherwise a 
				pointer to the first character of the first occurrence of needle
				is returned.
 */

char	*ft_strnstr(const char *haystack, const char *needle, size_t len)
{
	size_t	j;

	if (!haystack && !len)
		return (NULL);
	if (!*needle)
		return ((char *)haystack);
	while (*haystack && len--)
	{
		j = 0;
		while (*(haystack + j) == *(needle + j)
			&& *(needle + j) && j <= len)
		{
			if (!*(needle + j + 1))
				return ((char *)haystack);
			j++;
		}
		haystack++;
	}
	return (NULL);
}
