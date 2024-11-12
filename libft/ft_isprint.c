/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isprint.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: spenev <spenev@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/13 11:34:54 by spenev            #+#    #+#             */
/*   Updated: 2023/12/04 10:31:02 by spenev           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* 
	DESCRIPTION:
				The isprint() function tests for any printing character, 
				including space (‘ ’). The value of the argument must be 
				representable as an unsigned char

	RETURN VALUES:
				The isprint() function returns zero if the character tests false
				and returns non-zero if the character tests true.
 */

int	ft_isprint(int c)
{
	if (c >= 32 && c <= 126)
		return (1);
	return (0);
}
