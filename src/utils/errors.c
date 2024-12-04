/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: spenev <spenev@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/04 10:56:46 by spenev            #+#    #+#             */
/*   Updated: 2024/12/04 10:57:14 by spenev           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_print_error(char *prefix, char *dynamic_part, char *suffix)
{
	if (prefix)
		ft_putstr_fd(prefix, STDERR_FILENO);
	if (dynamic_part)
		ft_putstr_fd(dynamic_part, STDERR_FILENO);
	if (suffix)
		ft_putstr_fd(suffix, STDERR_FILENO);
}
