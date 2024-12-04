/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anilchen <anilchen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/26 13:33:29 by anilchen          #+#    #+#             */
/*   Updated: 2024/12/04 15:04:22 by anilchen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "process.h"

// grep hi <./test_files/empty <./test_files/infile

// Processes all redirections associated with a command.
// Iterates through the command's redirection list and applies the
// appropriate redirection handler
// based on the type of redirection (output, append, input, heredoc).
// Parameters:
// - cmd: Pointer to the command structure containing the redirections.
// Returns:
// - 0: If all redirections are successfully handled.
// -1: If any redirection fails or an unknown redirection type is encountered.
// Notes:
// - Supported redirection types:
// - REDIRECT_OUTPUT: Handles output redirection (`redir_output`).
// - REDIRECT_APPEND: Handles append redirection (`redir_append`).
// - REDIRECT_INPUT: Handles input redirection (`redir_input`).
// - HEREDOC: Placeholder for heredoc handling logic.
// - Logs errors for unsupported redirection types or failures.

int	redir_out(t_redirection *redir)
{
	if (redir->type == REDIRECT_OUTPUT)
	{
		if (redir_output(redir) == -1)
			return (-1);
	}
	else if (redir->type == REDIRECT_APPEND)
	{
		if (redir_append(redir) == -1)
			return (-1);
	}
	return (0);
}

int	redir_in(t_redirection *redir, t_process *process)
{
	if (redir->type == REDIRECT_INPUT)
	{
		if (redir_input(redir) == -1)
			return (-1);
	}
	else if (redir->type == HEREDOC)
	{
		if (redir->was_processed)
			return (0);
		if (heredoc(redir, process) == -1)
			return (-1);
	}
	return (0);
}

int	handle_redirections(t_command *cmd, t_process *process)
{
	t_redirection	*redir;

	redir = cmd->redirections;
	while (redir)
	{
		if (redir->type == REDIRECT_OUTPUT || redir->type == REDIRECT_APPEND)
		{
			if (redir_out(redir) == -1)
				return (-1);
		}
		else if (redir->type == REDIRECT_INPUT || redir->type == HEREDOC)
		{
			if (redir_in(redir, process) == -1)
				return (-1);
		}
		else
		{
			write(2, "Unknown redirection type\n", 25);
			return (-1);
		}
		redir = redir->next;
	}
	return (0);
}
