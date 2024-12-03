/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections_2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anilchen <anilchen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/29 17:34:56 by anilchen          #+#    #+#             */
/*   Updated: 2024/12/03 16:08:31 by anilchen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "process.h"

// Handles output redirection (e.g., `> filename`).
// Opens the specified file in write mode, truncating it if it already exists,
// and redirects the command's standard output to the file.
// Parameters:
// - redir: Pointer to the redirection structure containing the target
// filename.
// Returns:
// - 0: If the redirection is successful.
// - -1: If an error occurs during file opening or redirection.
// Notes:
//	- Logs errors using `perror` and provides the filename that caused the
// issue.
// - Closes the file descriptor after duplication to avoid resource leaks.

int	redir_output(t_redirection *redir)
{
	int	fd;

	fd = open(redir->filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd < 0)
	{
		write(2, "minishell: ", 11);
		perror(redir->filename);
		return (-1);
	}
	if (dup2(fd, STDOUT_FILENO) == -1)
	{
		perror("dup2");
		close(fd);
		return (-1);
	}
	close(fd);
	return (0);
}

// Handles output redirection in append mode (e.g., `>> filename`).
// Opens the specified file in write mode,
//	appending new content to the file if it already exists,
// and redirects the command's standard output to the file.
// Parameters:
// - redir: Pointer to the redirection structure containing the target filename.
// Returns:
// - 0: If the redirection is successful.
// - -1: If an error occurs during file opening or redirection.
// Notes:
// - Logs errors using `perror` and provides the filename that caused the issue.
// - Closes the file descriptor after duplication to avoid resource leaks.

int	redir_append(t_redirection *redir)
{
	int	fd;

	fd = open(redir->filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (fd < 0)
	{
		write(2, "minishell: ", 11);
		perror(redir->filename);
		return (-1);
	}
	if (dup2(fd, STDOUT_FILENO) == -1)
	{
		perror("dup2");
		close(fd);
		return (-1);
	}
	close(fd);
	return (0);
}

// Handles input redirection (e.g., `< filename`).
// Ensures the specified file exists and is readable,
// then redirects the command's standard input to the file.
// Parameters:
//	- redir: Pointer to the redirection structure containing the target
// filename.
// Returns:
// - 0: If the redirection is successful.
// - -1: If the file does not exist, is not readable, or redirection fails.
// Notes:
// - Logs errors using `perror` and provides the filename that caused the issue.
// - Closes the file descriptor after duplication to avoid resource leaks.

int	redir_input(t_redirection *redir)
{
	int	fd;

	if (access(redir->filename, F_OK | R_OK) != 0)
	{
		write(2, "minishell: ", 11);
		perror(redir->filename);
		return (-1);
	}
	fd = open(redir->filename, O_RDONLY);
	if (fd < 0)
	{
		write(2, "minishell: ", 11);
		perror(redir->filename);
		return (-1);
	}
	if (dup2(fd, STDIN_FILENO) == -1)
	{
		perror("dup2");
		close(fd);
		return (-1);
	}
	close(fd);
	return (0);
}
