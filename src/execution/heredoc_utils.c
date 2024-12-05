/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stfn <stfn@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/05 23:37:00 by stfn              #+#    #+#             */
/*   Updated: 2024/12/05 23:44:19 by stfn             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "process.h"
#include "minishell.h"

// Function to handle missing delimiter
int	handle_missing_delimiter(t_redirection *redir)
{
	printf("Error: Expected heredoc delimiter '%s' not found before \
        end of input.\n", redir->filename);
	return (0);
}

// TO DO the getpid() need to be replaced
int	prepare_temp_file(char *temp_file, t_process *process)
{
	char	*pid_str;

	pid_str = ft_itoa(getpid());
	if (!pid_str)
	{
		perror("heredoc: ft_itoa");
		set_exit_status(process, 1);
		return (-1);
	}
	ft_strcpy(temp_file, "/tmp/heredoc_");
	ft_strcat(temp_file, pid_str);
	free(pid_str);
	return (0);
}

int	open_temp_file(const char *temp_file)
{
	int	fd;

	fd = open(temp_file, O_CREAT | O_WRONLY | O_TRUNC, 0600);
	if (fd == -1)
	{
		perror("heredoc: open");
		exit(1);
	}
	return (fd);
}

void	process_heredoc_lines(int fd, t_redirection *redir)
{
	char	*line;

	while (1)
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
			break ;
		}
		write(fd, line, ft_strlen(line));
		write(fd, "\n", 1);
		free(line);
	}
	close(fd);
	exit(0);
}

int	process_exit_status(const char *temp_file, t_process *process, int status)
{
	int	sig;
	int	exit_status;

	if (WIFEXITED(status))
	{
		exit_status = WEXITSTATUS(status);
		if (exit_status != 0)
		{
			unlink(temp_file);
			set_exit_status(process, exit_status);
			return (-1);
		}
		set_exit_status(process, 0);
	}
	else if (WIFSIGNALED(status))
	{
		sig = WTERMSIG(status);
		unlink(temp_file);
		set_exit_status(process, 128 + sig);
		return (-1);
	}
	return (0);
}
