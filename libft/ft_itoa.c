/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: spenev <spenev@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/14 09:01:15 by spenev            #+#    #+#             */
/*   Updated: 2023/12/04 10:29:00 by spenev           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* 
	DESCRIPTION:
				Allocates (with malloc(3)) and returns a string representing 
				the integer received as an argument. Negative numbers must be 
				handled.

	RETURN VALUES:
				The string representing the integer. NULL if the allocation 
				fails.
 */

#include "libft.h"

static int	count_digits(int n)
{
	int	count;

	if (n == 0)
		return (1);
	count = 0;
	if (n < 0)
	{
		n *= -1;
		count++;
	}
	while (n > 0)
	{
		n /= 10;
		count++;
	}
	return (count);
}

char	*ft_itoa(int n)
{
	int		length;
	char	*buffer;

	if (n == -2147483648)
		return (ft_strdup("-2147483648"));
	length = count_digits(n);
	buffer = (char *)malloc(length + 1);
	if (buffer == NULL)
		return (NULL);
	buffer[0] = '0';
	if (n < 0)
	{
		buffer[0] = '-';
		n *= -1;
	}
	buffer[length] = '\0';
	length--;
	while (n > 0)
	{
		buffer[length] = (n % 10) + '0';
		n /= 10;
		length--;
	}
	return (buffer);
}
