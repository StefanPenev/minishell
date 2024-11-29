/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_utils_2.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anilchen <anilchen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/20 16:44:21 by anilchen          #+#    #+#             */
/*   Updated: 2024/11/29 15:42:49 by anilchen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "process.h"

// Swaps the keys and values of two nodes in a linked list.
// The pointers to `key` and `value` in `node1` and `node2` are exchanged.
// This function does not modify the `next` pointers of the nodes.

void	swap_nodes(t_env *node1, t_env *node2)
{
	char	*temp_key;
	char	*temp_value;

	if (node1 == node2)
		return ;
	(temp_key = node1->key);
	temp_value = node1->value;
	node1->key = node2->key;
	node1->value = node2->value;
	node2->key = temp_key;
	node2->value = temp_value;
}

// Checks if the given path points to an executable file.
// Returns 1 if the path is executable and a regular file, otherwise returns 0.

int	is_executable(const char *path)
{
	struct stat	sb;

	if (stat(path, &sb) == 0 && access(path, X_OK) == 0 && S_ISREG(sb.st_mode))
		return (1);
	return (0);
}

// Parses the sign from the string and skips leading whitespace.
// Returns the sign as 1 (positive) or -1 (negative).
// Updates the pointer to the first numeric character.
int	parse_sign_and_skip_whitespace(const char **str)
{
	int	sign;

	sign = 1;
	while (**str == ' ' || **str == '\t' || **str == '\n' || **str == '\v'
		|| **str == '\f' || **str == '\r')
		(*str)++;
	if (**str == '-')
	{
		sign = -1;
		(*str)++;
	}
	else if (**str == '+')
		(*str)++;
	return (sign);
}

// Converts the numeric characters in a string to a long value.
// Detects overflow and sets the overflow flag if it occurs.
long	convert_to_long(const char *str, int sign, int *overflow)
{
	long	result;

	result = 0;
	*overflow = 0;
	while (*str >= '0' && *str <= '9')
	{
		if (result > (LONG_MAX - (*str - '0')) / 10)
		{
			*overflow = 1;
			if (sign == 1)
				return (LONG_MAX);
			else
				return (LONG_MIN);
		}
		result = result * 10 + (*str - '0');
		str++;
	}
	return (result * sign);
}

// Converts a string to a long integer, handling overflow and underflow.
// Skips leading whitespace and parses the sign before converting the
// numeric part.

long	ft_atol(const char *str, int *overflow)
{
	int	sign;

	sign = parse_sign_and_skip_whitespace(&str);
	return (convert_to_long(str, sign, overflow));
}
