/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memset.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: spenev <spenev@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/13 11:35:10 by spenev            #+#    #+#             */
/*   Updated: 2023/11/13 11:55:25 by spenev           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string.h>

/* 
	DESCRIPTION:
				The memset() function writes len bytes of value c 
                (converted to an unsigned char) to the string b.

	RETURN VALUES:
				The memset() function returns its first argument.
 */

void	*ft_memset(void *b, int c, size_t len)
{
	size_t	i;

	i = 0;
	while (i < len)
	{
		((unsigned char *)b)[i] = c;
		i++;
	}
	return (b);
}
