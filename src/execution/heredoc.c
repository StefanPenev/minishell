/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anilchen <anilchen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/18 17:05:03 by anilchen          #+#    #+#             */
/*   Updated: 2024/12/06 12:47:57 by anilchen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "process.h"
#include "signals.h"

int	handle_heredoc_child(t_redirection *redir, const char *temp_file)
{
	int	fd;

	fd = open_temp_file(temp_file);
	process_heredoc_lines(fd, redir);
	return (0);
}

int	execute_heredoc_child(t_redirection *redir, const char *temp_file)
{
	sig_heredoc();
	suppress_output();
	handle_heredoc_child(redir, temp_file);
	return (0);
}

int	handle_heredoc_parent(const char *temp_file, t_process *process,
	pid_t pid)
{
	int	status;

	signal(SIGINT, SIG_IGN);
	waitpid(pid, &status, 0);
	sig_init();
	return (process_exit_status(temp_file, process, status));
}

int	open_and_finalize_heredoc(const char *temp_file, t_redirection *redir,
	t_process *process)
{
	int	temp_fd;

	temp_fd = open(temp_file, O_RDONLY);
	if (temp_fd == -1)
	{
		perror("heredoc: open");
		unlink(temp_file);
		set_exit_status(process, 1);
		return (-1);
	}
	redir->fd = temp_fd;
	unlink(temp_file);
	redir->was_processed = 1;
	return (0);
}

int	heredoc(t_redirection *redir, t_process *process)
{
	pid_t	pid;
	char	temp_file[256];

	if (prepare_temp_file(temp_file, process) == -1)
		return (-1);
	pid = fork();
	if (pid == -1)
	{
		perror("heredoc: fork");
		set_exit_status(process, 1);
		return (-1);
	}
	if (pid == 0)
		execute_heredoc_child(redir, temp_file);
	else
	{
		if (handle_heredoc_parent(temp_file, process, pid) == -1)
			return (-1);
		return (open_and_finalize_heredoc(temp_file, redir, process));
	}
	return (0);
}

// Heredoc function
// int	heredoc(t_redirection *redir, t_process *process)
// {
// 	int		temp_fd;
// 	pid_t	pid;
// 	int		status;
// 	char	temp_file[256];
// 	char	*pid_str;

// 	pid_str = ft_itoa(getpid());
// 	if (!pid_str)
// 	{
// 		perror("heredoc: ft_itoa");
// 		set_exit_status(process, 1);
// 		return (-1);
// 	}
// 	ft_strcpy(temp_file, "/tmp/heredoc_");
// 	ft_strcat(temp_file, pid_str);
// 	free(pid_str);
// 	pid = fork();
// 	if (pid == -1)
// 	{
// 		perror("heredoc: fork");
// 		set_exit_status(process, 1);
// 		return (-1);
// 	}
// 	if (pid == 0)
// 	{
// 		int		fd;
// 		char	*line;

// 		sig_heredoc();
// 		suppress_output();
// 		fd = open(temp_file, O_CREAT | O_WRONLY | O_TRUNC, 0600);
// 		if (fd == -1)
// 		{
// 			perror("heredoc: open");
// 			exit(1);
// 		}
// 		while (1)
// 		{
// 			line = readline("heredoc> ");
// 			if (!line)
// 			{
// 				handle_missing_delimiter(redir);
// 				close(fd);
// 				exit(0);
// 			}
// 			if (strcmp(line, redir->filename) == 0)
// 			{
// 				free(line);
// 				break ;
// 			}
// 			write(fd, line, strlen(line));
// 			write(fd, "\n", 1);
// 			free(line);
// 		}
// 		close(fd);
// 		exit(0);
// 	}
// 	else
// 	{
// 		signal(SIGINT, SIG_IGN);
// 		waitpid(pid, &status, 0);
// 		sig_init();
// 		if (WIFEXITED(status))
// 		{
// 			int exit_status = WEXITSTATUS(status);
// 			if (exit_status != 0)
// 			{
// 				unlink(temp_file);
// 				set_exit_status(process, exit_status);
// 				return (-1);
// 			}
// 			else
// 				set_exit_status(process, 0);
// 		}
// 		else if (WIFSIGNALED(status))
// 		{
// 			int sig = WTERMSIG(status);
// 			unlink(temp_file);
// 			set_exit_status(process, 128 + sig);
// 			return (-1);
// 		}
// 		temp_fd = open(temp_file, O_RDONLY);
// 		if (temp_fd == -1)
// 		{
// 			perror("heredoc: open");
// 			unlink(temp_file);
// 			set_exit_status(process, 1);
// 			return (-1);
// 		}
// 		redir->fd = temp_fd;
// 		unlink(temp_file);
// 		redir->was_processed = 1;
// 		return (0);
// 	}
// }
