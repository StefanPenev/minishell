/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: spenev <spenev@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/13 11:35:17 by spenev            #+#    #+#             */
/*   Updated: 2023/12/04 10:33:51 by spenev           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/* 
	DESCRIPTION: 
				The strlcat() function copy and concatenate strings with the
				same input parameters and output result as snprintf(3).

   RETURN VALUES: 
   				The strlcat() function return the total length of the string
				tried to create. That means the initial length of dst plus the 
				length of src. 
*/

size_t	ft_strlcat(char *dst, const char *src, size_t dstsize)
{
	size_t		d_len;
	size_t		total_len;
	const char	*s;

	if ((!dst || !src) && !dstsize)
		return (0);
	s = src;
	d_len = 0;
	while (*(dst + d_len) && d_len < dstsize)
		d_len++;
	if (d_len < dstsize)
		total_len = d_len + ft_strlen(s);
	else
		return (dstsize + ft_strlen(s));
	while (*s && (d_len + 1) < dstsize)
	{
		*(dst + d_len) = *s++;
		d_len++;
	}
	*(dst + d_len) = '\0';
	return (total_len);
}
