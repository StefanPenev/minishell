/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_bzero.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: spenev <spenev@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/13 11:33:30 by spenev            #+#    #+#             */
/*   Updated: 2023/11/13 11:34:11 by spenev           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/* 
	DESCRIPTION:
				The bzero() function writes n zeroed bytes to the string s.
				If n is zero, bzero() does nothing.
 */

void	ft_bzero(void *s, size_t n)
{
	ft_memset(s, '\0', n);
}
