/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anilchen <anilchen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/26 13:33:29 by anilchen          #+#    #+#             */
/*   Updated: 2024/11/28 20:03:25 by anilchen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "process.h"

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

int	redir_input(t_redirection *redir)
{
	int	fd;

	if (access(redir->filename, F_OK | R_OK) != 0)
	{
		perror("access");
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

// grep hi <./test_files/empty <./test_files/infile
int	handle_redirections(t_command *cmd)
{
	t_redirection	*redir;
	t_redirection	*last_input;
	t_redirection	*last_output;

	int count_input = 0; // debug
	int count_output = 0; // debug
	redir = cmd->redirections;
	last_input = NULL;
	while (redir)
	{
		if (redir->type == REDIRECT_INPUT)
		{
			printf("[DEBUG] Parsed redirection: type = %d, number: %d, file = %s\n", redir->type, count_input, redir->filename);
			last_input = redir;
			count_input++; // Debug
		}
		if (redir->type == REDIRECT_OUTPUT || redir->type == REDIRECT_APPEND)
		{
			printf("[DEBUG] Parsed redirection: type = %d, number: %d, file = %s\n", redir->type, count_output, redir->filename);
			last_output = redir;
			count_output++; // Debug
		}
		redir = redir->next;
	}
	redir = cmd->redirections;
	while (redir)
	{
		printf("[DEBUG] Handling last input: type = %d, file = %s\n",
			last_input->type, last_input->filename);
		printf("[DEBUG] Handling redirection: type = %d, file = %s\n",
			redir->type, redir->filename);
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
		else if (redir->type == REDIRECT_INPUT && redir == last_input)
		{
			if (redir_input(redir) == -1)
				return (-1);
		}
		else if (redir->type == HEREDOC)
		{
			// code ../
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

// int	handle_redirections(t_command *cmd)
// {
// 	t_redirection	*redir;

// 	redir = cmd->redirections;
// 	while (redir)
// 	{
// 		if (redir->type == REDIRECT_OUTPUT)
// 		{
// 			if (redir_output(redir) == -1)
// 				return (-1);
// 		}
// 		else if (redir->type == REDIRECT_APPEND)
// 		{
// 			if (redir_append(redir) == -1)
// 				return (-1);
// 		}
// 		else if (redir->type == REDIRECT_INPUT)
// 		{
// 			while (redir->type == REDIRECT_INPUT && redir->next != NULL)
// 			{
// 				redir = redir->next;
// 			}
// 			if (redir_input(redir) == -1)
// 				return (-1);
// 		}
// 		else if (redir->type == HEREDOC)
// 		{
// 			// code..
// 		}
// 		else
// 		{
// 			printf("Unknown redirection type\n");
// 			return (-1);
// 		}
// 		redir = redir->next;
// 	}
// 	return (0);
// }
