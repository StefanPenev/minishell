/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes_utils_2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anilchen <anilchen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 16:36:38 by anilchen          #+#    #+#             */
/*   Updated: 2024/11/27 19:05:36 by anilchen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "process.h"

// int	create_fork(pid_t *pid, t_process *process)
// {
// 	*pid = fork();
// 	if (*pid > 0)
// 	{
// 		printf("[DEBUG] Родительский процесс для команды. PID: %d\n", *pid);
// 	}
// 	else if (*pid == 0)
// 	{
// 		printf("[DEBUG] Дочерний процесс создан. PID: %d\n", getpid());
// 	}
// 	if (*pid == -1)
// 	{
// 		perror("[ERROR] Fork failed");
// 		set_exit_status(process, 1);
// 		return (-1);
// 	}
// 	return (EXIT_SUCCESS);
// }

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
