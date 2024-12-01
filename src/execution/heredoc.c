/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stfn <stfn@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/18 17:05:03 by anilchen          #+#    #+#             */
/*   Updated: 2024/12/01 17:15:12 by stfn             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "process.h"
#include "minishell.h"
#include "signals.h"

extern volatile sig_atomic_t	g_heredoc_interrupted;

// Function to handle missing delimiter
int	handle_missing_delimiter(t_redirection *redir)
{
	fprintf(stderr, "Error: Expected heredoc delimiter '%s' not found before end of input.\n", redir->filename);
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

// Heredoc function
int	heredoc(t_redirection *redir)
{
	int		temp_fd;
	pid_t	pid;
	int		status;
	char	temp_file[256];
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
	pid = fork();
	if (pid == -1)
	{
		perror("heredoc: fork");
		return (-1);
	}
	if (pid == 0)
	{
		int		fd;
		char	*line;

		sig_heredoc();
		suppress_output();
		fd = open(temp_file, O_CREAT | O_WRONLY | O_TRUNC, 0600);
		if (fd == -1)
		{
			perror("heredoc: open");
			exit(1);
		}
		while (1)
		{
			line = readline("heredoc> ");
			if (!line)
			{
				handle_missing_delimiter(redir);
				close(fd);
				exit(0);
			}
			if (strcmp(line, redir->filename) == 0)
			{
				free(line);
				break ;
			}
			write(fd, line, strlen(line));
			write(fd, "\n", 1);
			free(line);
		}
		close(fd);
		exit(0);
	}
	else
	{
		signal(SIGINT, SIG_IGN);
		waitpid(pid, &status, 0);
		sig_init();
		if (WIFEXITED(status))
		{
			int exit_status = WEXITSTATUS(status);
			if (exit_status != 0)
			{
				unlink(temp_file);
				return (-1);
			}
		}
		else if (WIFSIGNALED(status))
		{
			unlink(temp_file);
			return (-1);
		}
		temp_fd = open(temp_file, O_RDONLY);
		if (temp_fd == -1)
		{
			perror("heredoc: open");
			unlink(temp_file);
			return (-1);
		}
		redir->fd = temp_fd;
		unlink(temp_file);
		return (0);
	}
}
