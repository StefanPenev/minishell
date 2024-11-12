/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_tolower.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: spenev <spenev@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/13 11:35:32 by spenev            #+#    #+#             */
/*   Updated: 2023/11/13 12:06:06 by spenev           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* 
	DESCRIPTION:
				The tolower() function converts an upper-case letter to the 
				corresponding lower-case letter.

	RETURN VALUES:
				If the argument is an upper-case letter, the tolower()
				function returns the corresponding lower-case letter if 
				there is one; otherwise, the argument is returned unchanged.
 */

int	ft_tolower(int c)
{
	if (c >= 'A' && c <= 'Z')
		return (c += 32);
	return (c);
}
