/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes_utils_3.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anilchen <anilchen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/03 14:01:12 by anilchen          #+#    #+#             */
/*   Updated: 2024/12/03 14:01:44 by anilchen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "process.h"

// Duplicates a file descriptor to a specific stream (e.g., stdin, stdout,
// stderr). Uses dup2 to redirect the given file descriptor (fd) to the
// target stream (n). If dup2 fails,
// it prints an error message and exits the program with a failure status.
// Parameters:
//   - fd: The source file descriptor to duplicate.
//   - n: The target file descriptor to overwrite (e.g., STDIN_FILENO,
//	STDOUT_FILENO).
// Notes:
//   - If fd is negative,
//	the function does nothing (no redirection is performed).
//   - If dup2 fails, perror is used to log the error,
//	and the program exits immediately to avoid undefined behavior.

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

// Safely closes a file descriptor if it is valid (greater than or equal to 0).
// Prevents attempts to close an invalid file descriptor, which could
// lead to errors or undefined behavior.
// Parameters:
//   - fd: The file descriptor to close.
// Notes:
//   - If fd is negative, the function does nothing.
//   - This is a helper function to simplify file descriptor management
// and ensure robust error handling.

void	close_safe(int fd)
{
	if (fd >= 0)
		close(fd);
}
