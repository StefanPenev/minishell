/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stfn <stfn@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/18 17:05:03 by anilchen          #+#    #+#             */
/*   Updated: 2024/12/05 01:23:42 by stfn             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "process.h"
#include "signals.h"

extern volatile sig_atomic_t	g_heredoc_interrupted;

// Function to handle missing delimiter
int	handle_missing_delimiter(t_redirection *redir)
{
	fprintf(stderr,
		"Error: Expected heredoc delimiter '%s' not found before end of input.\n",
		redir->filename);
	return (0);
}

// Function to open temporary file
int	open_temp_file(char *temp_file)
{
	int	temp_fd;

	temp_fd = open(temp_file, O_CREAT | O_RDWR | O_TRUNC, 0600);
	if (temp_fd == -1)
	{
		perror("open");
		exit(1);
	}
	return (temp_fd);
}

// Function to create unique temporary file name
void	create_temp_file(char *temp_file)
{
	pid_t	pid;

	pid = getpid();
	snprintf(temp_file, 64, "/tmp/heredoc_%d", pid);
}

// Function to read a string with prompt
char	*read_string(const char *prompt)
{
	char	*input;

	input = readline(prompt);
	if (!input)
		return (NULL);
	return (input);
}

int	setup_temp_file(char *temp_file)
{
	char	*pid_str;

	pid_str = ft_itoa(getpid());
	if (!pid_str)
	{
		perror("heredoc: ft_itoa");
		return (-1);
	}
	ft_strcpy(temp_file, "/tmp/heredoc_");
	ft_strcat(temp_file, pid_str);
	free(pid_str);
	return (0);
}

int	read_line(int fd, char *line, t_redirection *redir)
{
	line = readline("heredoc> ");
	if (!line)
	{
		handle_missing_delimiter(redir);
		close(fd);
		exit(0);
	}
	if (ft_strcmp(line, redir->filename) == 0)
	{
		free(line);
		return (1);
	}
	write(fd, line, ft_strlen(line));
	write(fd, "\n", 1);
	free(line);
	return (0);
}

void	heredoc_child_process(char *temp_file, t_redirection *redir)
{
	int		fd;
	char	*line;

	line = NULL;
	sig_heredoc();
	fd = open(temp_file, O_CREAT | O_WRONLY | O_TRUNC, 0600);
	if (fd == -1)
	{
		perror("heredoc: open");
		exit(1);
	}
	while (read_line(fd, line, redir) == 0)
	{
	}
	close(fd);
	exit(0);
}

int	heredoc_parent_process(char *temp_file, t_redirection *redir, pid_t pid,
		t_process *process)
{
	int	fd;

	signal(SIGINT, SIG_IGN);
	handle_child_exit_status(pid, process);
	if (process->last_exit_status != 0)
	{
		unlink(temp_file);
		return (-1);
	}
	fd = open(temp_file, O_RDONLY);
	if (fd == -1)
	{
		perror("heredoc: open");
		unlink(temp_file);
		return (-1);
	}
	redir->fd = fd;
	unlink(temp_file);
	redir->was_processed = 1;
	return (0);
}

int	heredoc(t_redirection *redir, t_process *process)
{
	pid_t	pid;
	char	temp_file[256];

	if (setup_temp_file(temp_file) == -1)
		return (-1);
	pid = fork();
	if (pid == -1)
	{
		perror("heredoc: fork");
		return (-1);
	}
	if (pid == 0)
		heredoc_child_process(temp_file, redir);
	else
		heredoc_parent_process(temp_file, redir, pid, process);
	return (0);
}

// int	heredoc(t_redirection *redir, t_process *process)
// {
// 	pid_t	pid;
// 	char	temp_file[256];
// 	char	*pid_str;
// 	int		fd;

// 	char	*line;
// 	pid_str = ft_itoa(getpid());
// 	if (!pid_str)
// 	{
// 		perror("heredoc: ft_itoa");
// 		return (-1);
// 	}
// 	ft_strcpy(temp_file, "/tmp/heredoc_");
// 	ft_strcat(temp_file, pid_str);
// 	free(pid_str);
// 	pid = fork();
// 	if (pid == -1)
// 	{
// 		perror("heredoc: fork");
// 		return (-1);
// 	}
// 	if (pid == 0)
// 	{
// 		sig_heredoc();
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
// 			if (ft_strcmp(line, redir->filename) == 0)
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
// 		handle_child_exit_status(pid, process);
// 		if (process->last_exit_status != 0)
// 		{
// 			unlink(temp_file);
// 			return (-1);
// 		}
// 		fd = open(temp_file, O_RDONLY);
// 		if (fd == -1)
// 		{
// 			perror("heredoc: open");
// 			unlink(temp_file);
// 			return (-1);
// 		}
// 		redir->fd = fd;
// 		unlink(temp_file);
// 		redir->was_processed = 1;
// 	}
// 	return (0);
// }

// Heredoc function
// int	heredoc(t_redirection *redir, t_process *process)
// {
// 	//int		temp_fd;
// 	pid_t	pid;
// 	//int		status;
// 	char	temp_file[256];
// 	char	*pid_str;
// 		int fd;
// 		char *line;

// 	pid_str = ft_itoa(getpid());
// 	if (!pid_str)
// 	{
// 		perror("heredoc: ft_itoa");
// 		return (-1);
// 	}
// 	ft_strcpy(temp_file, "/tmp/heredoc_");
// 	ft_strcat(temp_file, pid_str);
// 	free(pid_str);
// 	pid = fork();
// 	if (pid == -1)
// 	{
// 		perror("heredoc: fork");
// 		return (-1);
// 	}
// 	if (pid == 0)
// 	{
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
// 		sig_init();
// 		//waitpid(pid, &status, 0);
// 		//printf("status: %d\n", status);
// 		handle_child_exit_status(pid, process);
// 			// if (WIFEXITED(status))
// 			// {
// 			// 	int exit_status = WEXITSTATUS(status);
// 			// 	if (exit_status != 0)
// 			// 	{
// 			// 		unlink(temp_file);
// 			// 		return (-1);
// 			// 	}
// 			// }
// 			// else if (WIFSIGNALED(status))
// 			// {
// 			// 	set_exit_status(process, 130);
// 			// 	unlink(temp_file);
// 			// 	return (-1);
// 			// }
// 		redir->fd = open(temp_file, O_RDONLY);
// 		if (redir->fd == -1)
// 		{
// 			perror("heredoc: open");
// 			unlink(temp_file);
// 			return (-1);
// 		}
// 		//redir->fd = temp_fd;
// 		unlink(temp_file);
// 		redir->was_processed = 1; // anna
// 		return (0);
// 	}
// }
