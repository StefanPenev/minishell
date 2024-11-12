/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strncmp.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: spenev <spenev@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/13 11:35:25 by spenev            #+#    #+#             */
/*   Updated: 2023/12/04 10:34:11 by spenev           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/*
	DESCRIPTION:
				The strncmp() function compares not more than n characters.
				Because strncmp() is designed for comparing strings rather 
				than binary data, characters that appear after a ‘\0’
     			character are not compared.

	RETURN VALUES:
				The strncmp() function return an integer greater than, equal to, 
				or less than 0, according as the string s1 is greater than, 
				equal to, or less than the string s2. The comparison is done 
				using unsigned characters
 */

int	ft_strncmp(const char *s1, const char *s2, size_t n)
{
	unsigned int	i;

	i = 0;
	while ((s1[i] != '\0' || s2[i] != '\0') && n--)
	{
		if (s1[i] != s2[i])
			return ((unsigned char)s1[i] - (unsigned char)s2[i]);
		i++;
	}
	return (0);
}
