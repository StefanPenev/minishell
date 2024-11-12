/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: spenev <spenev@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/13 11:35:07 by spenev            #+#    #+#             */
/*   Updated: 2023/11/13 11:54:28 by spenev           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/* 
	DESCRIPTION:
				The memmove() function copies len bytes from string src to 
				string dst. The two strings may overlap; the copy is always 
				done in a non-destructive manner.

	RETURN VALUES:
				The memmove() function returns the original value of dst.
 */

void	*ft_memmove(void *dst, const void *src, size_t len)
{
	unsigned char	*temp_dst;
	unsigned char	*temp_src;

	temp_dst = dst;
	temp_src = (unsigned char *)src;
	if (dst < src)
		return (ft_memcpy(dst, src, len));
	if (dst > src)
		while (len--)
			temp_dst[len] = temp_src[len];
	return (dst);
}
