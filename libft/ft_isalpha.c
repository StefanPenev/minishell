/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isalpha.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: spenev <spenev@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/13 11:03:35 by spenev            #+#    #+#             */
/*   Updated: 2023/12/04 10:30:34 by spenev           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/* 
	DESCRIPTION:
				The isalpha() function tests for any character for which 
				isupper(3) or islower(3) is true.  The value of the argument must
				be representable as an unsigned char

	RETURN VALUES:
				The isalpha() function returns zero if the character tests false
				and returns non-zero if the character tests true.
 */

int	ft_isalpha(int c)
{
	if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'))
		return (1);
	return (0);
}
