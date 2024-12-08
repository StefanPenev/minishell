/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stefan <stefan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/04 10:56:46 by spenev            #+#    #+#             */
/*   Updated: 2024/12/08 22:59:17 by stefan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
 * Print an error message to standard error (stderr).
 * This function concatenates and prints the provided strings (prefix,
 * dynamic_part, and suffix) to stderr. Each part is printed only if it
 * is not NULL, allowing for flexible error message construction.
 */
void	ft_print_error(char *prefix, char *dynamic_part, char *suffix)
{
	if (prefix)
		ft_putstr_fd(prefix, STDERR_FILENO);
	if (dynamic_part)
		ft_putstr_fd(dynamic_part, STDERR_FILENO);
	if (suffix)
		ft_putstr_fd(suffix, STDERR_FILENO);
}
