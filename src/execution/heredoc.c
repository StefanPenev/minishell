/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stfn <stfn@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/18 17:05:03 by anilchen          #+#    #+#             */
/*   Updated: 2024/12/01 01:17:07 by stfn             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "process.h"
#include "minishell.h"

int	handle_missing_delimiter(t_redirection *redir)
{
    fprintf(stderr, "Error: Expected heredoc delimiter '%s' not found before end of input.\n", redir->filename);
	return (0);
}

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

void	create_temp_file(char *temp_file)
{
	pid_t	pid;

	pid = getpid();
	sprintf(temp_file, "/tmp/heredoc_%d", pid);
}

char	*read_string(void)
{
	char	*input;

	input = readline(NULL);
	if (!input)
		return (NULL);
	return (input);
}

int	heredoc(t_redirection *redir)
{
	char	*line;
	int		temp_fd;
	char	temp_file[64];

	create_temp_file(temp_file);
	temp_fd = open_temp_file(temp_file);
	while (1)
	{
		line = readline("heredoc> ");
		if (!line)
		{
			close(temp_fd);
			unlink(temp_file);
			return (handle_missing_delimiter(redir));
		}
		if (ft_strcmp(line, redir->filename) == 0)
		{
			free(line);
			break ;
		}
		write(temp_fd, line, ft_strlen(line));
		write(temp_fd, "\n", 1);
		free(line);
	}
	close(temp_fd);
	redir->fd = open(temp_file, O_RDONLY);
	if (redir->fd == -1)
	{
		perror("open");
		unlink(temp_file);
		return (-1);
	}
	unlink(temp_file);
	return (0);
}
