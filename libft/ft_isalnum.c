/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isalnum.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: spenev <spenev@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/13 11:34:41 by spenev            #+#    #+#             */
/*   Updated: 2023/12/04 10:30:07 by spenev           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/* 
	DESCRIPTION:
				The isalnum() function tests for any character for which 
				isalpha(3) or isdigit(3) is true. The value of the argument must
				be representable as an unsigned char

	RETURN VALUES:
				The isalnum() function returns zero if the character tests false 
				and returns non-zero if the character tests true.
 */

int	ft_isalnum(int c)
{
	if (ft_isalpha(c) || ft_isdigit(c))
		return (1);
	return (0);
}
