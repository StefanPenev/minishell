/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes_utils_2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anilchen <anilchen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 16:36:38 by anilchen          #+#    #+#             */
/*   Updated: 2024/11/28 16:52:15 by anilchen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "process.h"

void	close_safe(int fd)
{
	if (fd >= 0)
		close(fd);
}

void	dup_stream(int fd, int n)
{
	if (fd >= 0)
	{
		if (dup2(fd, n) == -1)
		{
			perror("dup2 failed");
			exit(EXIT_FAILURE);
		}
	}
}
