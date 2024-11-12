/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_toupper.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: spenev <spenev@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/13 11:35:35 by spenev            #+#    #+#             */
/*   Updated: 2023/11/13 12:07:02 by spenev           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* 
	DESCRIPTION:
				The toupper() function converts a lower-case letter to the 
				corresponding upper-case letter.

	RETURN VALUES:
				If the argument is a lower-case letter, the toupper() 
				function returns the corresponding upper-case letter if 
				there is one; otherwise, the argument is returned unchanged.
 */

int	ft_toupper(int c)
{
	if (c >= 'a' && c <= 'z')
		return (c -= 32);
	return (c);
}
