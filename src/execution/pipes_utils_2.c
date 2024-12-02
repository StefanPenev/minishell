/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes_utils_2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anilchen <anilchen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 16:36:38 by anilchen          #+#    #+#             */
/*   Updated: 2024/12/02 15:54:39 by anilchen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "process.h"

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

// Checks if the given command has an input redirection (e.g., `< filename`).
// Iterates through the command's redirections and looks for one with the type
//`REDIRECT_INPUT`.
// Parameters:
//   - cmd: A pointer to the command structure containing its redirection list.
// Returns:
//   - true: If an input redirection is found.
//   - false: If no input redirection is found.

bool	cmd_has_input_redirection(t_command *cmd)
{
	t_redirection	*redir;

	redir = cmd->redirections;
	while (redir)
	{
		// if (redir->type == REDIRECT_INPUT)
		if (redir->type == REDIRECT_INPUT || redir->type == HEREDOC)
			return (true);
		redir = redir->next;
	}
	return (false);
}

// Checks if the given command has an output redirection (e.g., `> filename`
// or `>> filename`).
// Iterates through the command's redirections and looks for one with the
// type `REDIRECT_OUTPUT`
// or `REDIRECT_APPEND`.
// Parameters:
//   - cmd: A pointer to the command structure containing its redirection list.
// Returns:
//   - true: If an output redirection is found.
//   - false: If no output redirection is found.

bool	cmd_has_output_redirection(t_command *cmd)
{
	t_redirection	*redir;

	redir = cmd->redirections;
	while (redir)
	{
		if (redir->type == REDIRECT_OUTPUT || redir->type == REDIRECT_APPEND)
			return (true);
		redir = redir->next;
	}
	return (false);
}

// Configures input and output streams for a middle command in a pipeline.
// If the command does not have input redirection, the input is taken
// from the previous pipe. If the command does not have output redirection,
// the output is sent to the next pipe.
// Closes all pipe file descriptors after setting up the streams.
// Parameters:
//	- fds: A pointer to the structure containing file descriptors for the
// current and previous pipes.
//  - cmd: A pointer to the command structure to configure.

void	middle_commands_streams(t_pipe_fds *fds, t_command *cmd)
{
	if (!cmd_has_input_redirection(cmd) && fds->fd_prev[0] >= 0)
		dup_stream(fds->fd_prev[0], STDIN_FILENO);
	if (!cmd_has_output_redirection(cmd) && fds->fd[1] >= 0)
		dup_stream(fds->fd[1], STDOUT_FILENO);
	close_safe(fds->fd_prev[0]);
	close_safe(fds->fd_prev[1]);
	close_safe(fds->fd[0]);
	close_safe(fds->fd[1]);
}

// Configures input and output streams for a command in a pipeline based on its
// position. Handles the redirection logic for the first, middle,
//	and last commands in a pipeline.
// Also applies any explicit redirections defined in the command (e.g., `<`,
//	`>`, `>>`).
// Parameters:
//- fds: A pointer to the structure containing file descriptors for the current
// and previous pipes.
//   - cmd: A pointer to the command structure to configure.
//   - flag: Indicates the position of the command in the pipeline:
//       - PIPE_FIRST: First command in the pipeline.
//       - PIPE_MIDDLE: Middle command in the pipeline.
//       - PIPE_LAST: Last command in the pipeline.

void	handle_streams(t_pipe_fds *fds, t_command *cmd, int flag)
{
	if (cmd && cmd->redirections)
	{
		if (handle_redirections(cmd) == -1)
		{
			printf("Error handling redirections\n");
			exit(EXIT_FAILURE);
		}
	}
	if (flag == PIPE_FIRST)
	{
		if (!cmd_has_output_redirection(cmd) && fds->fd[1] >= 0)
			dup_stream(fds->fd[1], STDOUT_FILENO);
		close_safe(fds->fd[0]);
		close_safe(fds->fd[1]);
	}
	else if (flag == PIPE_MIDDLE)
		middle_commands_streams(fds, cmd);
	else if (flag == PIPE_LAST)
	{
		if (!cmd_has_input_redirection(cmd) && fds->fd[0] >= 0)
			dup_stream(fds->fd[0], STDIN_FILENO);
		close_safe(fds->fd[0]);
		close_safe(fds->fd[1]);
	}
}
