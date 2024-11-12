/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlen.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: spenev <spenev@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/13 11:35:22 by spenev            #+#    #+#             */
/*   Updated: 2023/11/13 13:18:41 by spenev           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/* 
	DESCRIPTION:
				The strlen() function computes the length of the string s.
                The strnlen() function attempts to compute the length of s, 
                but never scans beyond the first maxlen bytes of s.
	
	RETURN VALUES:
				The strlen() function returns the number of characters that 
                precede the terminating NUL character.
 */

size_t	ft_strlen(const char *s)
{
	size_t	i;

	i = 0;
	while (s[i] != '\0')
		i++;
	return (i);
}
