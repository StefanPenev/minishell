/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anilchen <anilchen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/26 13:33:29 by anilchen          #+#    #+#             */
/*   Updated: 2024/11/27 15:05:26 by anilchen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "process.h"


// int	redir_output_pipes(t_redirection *redir)//, t_pipe_fds *fds)
// {
// 	int	fd;

// 	fd = open(redir->filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
// 	if (fd < 0)
// 	{
// 		write(2, "minishell: ", 11);
// 		perror(redir->filename);
// 		return (-1);
// 	}
// 	printf("[DEBUG(Redirection)] Redirecting stdout to file: %s, fd: %d\n", redir->filename, fd);

// 	// Применяем редирект к STDOUT_FILENO
// 	if (dup2(fd, STDOUT_FILENO) == -1)
// 	{
// 		perror("dup2");
// 		close(fd);
// 		return (-1);
// 	}
// 	close(fd);
// 	return (0);
// }

// int	redir_output_pipes(t_redirection *redir, t_pipe_fds *fds)
// {
// 	int	fd;

// 	fd = open(redir->filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	
// 	if (fd < 0)
// 	{
// 		write(2, "minishell: ", 11);
// 		perror(redir->filename);
// 		return (-1);
// 	}
// 	printf("[DEBUG(Redirection)] Redirecting stdout to fd[1]: %d\n", fd);
// 	if (fds->fd[1] >= 0)
// 	{
// 		if (dup2(fd, fds->fd[1]) == -1)
// 		{
// 			perror("dup2");
// 			close(fd);
// 			return (-1);
// 		}
// 	}
// 	else
// 	{
// 		if (dup2(fd, STDOUT_FILENO) == -1)
// 		{
// 			perror("dup2");
// 			close(fd);
// 			return (-1);
// 		}
// 	}
// 	close(fd);
// 	return (0);
// }

// int	redir_append_pipes(t_redirection *redir, t_pipe_fds *fds)
// {
// 	int	fd;

// 	fd = open(redir->filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
// 	if (fd < 0)
// 	{
// 		write(2, "minishell: ", 11);
// 		perror(redir->filename);
// 		return (-1);
// 	}
// 	if (fds->fd[1] >= 0)
// 	{
// 		if (dup2(fd, fds->fd[1]) == -1)
// 		{
// 			perror("dup2");
// 			close(fd);
// 			return (-1);
// 		}
// 	}
// 	else
// 	{
// 		if (dup2(fd, STDOUT_FILENO) == -1)
// 		{
// 			perror("dup2");
// 			close(fd);
// 			return (-1);
// 		}
// 	}
// 	close(fd);
// 	return (0);
// }

// int	redir_input_pipes(t_redirection *redir, t_pipe_fds *fds)
// {
// 	int	fd;

// 	if (access(redir->filename, F_OK | R_OK) != 0)
// 	{
// 		perror("access");
// 		return (-1);
// 	}
// 	fd = open(redir->filename, O_RDONLY);
// 	if (fd < 0)
// 	{
// 		write(2, "minishell: ", 11);
// 		perror(redir->filename);
// 		return (-1);
// 	}
// 	if (fds->fd[0] >= 0)
// 	{
// 		if (dup2(fd, fds->fd[0]) == -1)
// 		{
// 			perror("dup2");
// 			close(fd);
// 			return (-1);
// 		}
// 	}
// 	else
// 	{
// 		if (dup2(fd, STDIN_FILENO) == -1)
// 		{
// 			perror("dup2");
// 			close(fd);
// 			return (-1);
// 		}
// 	}
// 	close(fd);
// 	return (0);
// }

// int	handle_redirections_pipes(t_command *cmd, t_pipe_fds *fds)
// {
// 	t_redirection	*redir;

// 	redir = cmd->redirections;
// 	while (redir)
// 	{
// 		if (redir->type == REDIRECT_OUTPUT)
// 		{
// 			printf("call redir_output_pipes\n");
// 			if (redir_output_pipes(redir) == -1)
// 				return (-1);
// 		}
// 		else if (redir->type == REDIRECT_APPEND)
// 		{
// 			printf("call redir_append_pipes\n");
// 			if (redir_append_pipes(redir, fds) == -1)
// 				return (-1);
// 		}
// 		else if (redir->type == REDIRECT_INPUT)
// 		{
// 			printf("call redir_input_pipes\n");
// 			if (redir_input_pipes(redir, fds) == -1)
// 				return (-1);
// 		}
// 		else if (redir->type == HEREDOC)
// 		{
// 			// code
// 		}
// 		else
// 		{
// 			fprintf(stderr, "Unknown redirection type\n");
// 			return (-1);
// 		}
// 		redir = redir->next;
// 	}
// 	return (0);
// }

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

int	handle_redirections(t_command *cmd)
{
	t_redirection	*redir;

	redir = cmd->redirections;
	while (redir)
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
		else if (redir->type == REDIRECT_INPUT)
		{
			if (redir_input(redir) == -1)
				return (-1);
		}
		else if (redir->type == HEREDOC)
		{
			// code..
		}
		else
		{
			printf("Unknown redirection type\n");
			return (-1);
		}
		redir = redir->next;
	}
	return (0);
}
