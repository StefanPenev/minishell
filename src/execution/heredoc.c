/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stfn <stfn@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/18 17:05:03 by anilchen          #+#    #+#             */
/*   Updated: 2024/11/27 11:25:59 by stfn             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "process.h"
#include "minishell.h"

// static char	*read_string(void)
// {
// 	char	buffer[1024];
// 	ssize_t	bytes_read;

// 	bytes_read = read(STDIN_FILENO, buffer, sizeof(buffer) - 1);
// 	if (bytes_read <= 0)
// 		return (NULL);
// 	buffer[bytes_read] = '\0';
// 	return (ft_strdup(buffer));
// }
char	*read_string(void)
{
	char	*input;

	input = readline("");
	if (!input)
		return (NULL);
	return (input);
}


// void	here_doc(char *limiter, some_arguments)
// {
// 	char	*data;

// 	while (1)
// 	{
// 		ft_printf("heredoc> ");
// 		data = read_string();
// 		if (ft_strncmp(data, limiter, ft_strlen(limiter)) == 0
// 			&& data[ft_strlen(limiter)] == '\n')
// 		{
// 			free(data);
// 			break ;
// 		}
// 		write(arg_list->file1, data, ft_strlen(data));
// 		free(data);
// 	}
// }

void	create_temp_file(char *temp_file)
{
	char	*pid_str;

	pid_str = ft_itoa(getpid());
	if (pid_str == NULL)
	{
		perror("ft_itoa");
		exit(1);
	}
	strcpy(temp_file, "/tmp/heredoc_");
	strcat(temp_file, pid_str);
	free(pid_str);
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

void	write_to_temp_file(int temp_fd, char *limiter)
{
	char	*data;

	while (1)
	{
		printf("heredoc> ");
		data = readline(NULL);
		if (!data)
			break ;
		if (strcmp(data, limiter) == 0)
		{
			free(data);
			break ;
		}
		write(temp_fd, data, strlen(data));
		write(temp_fd, "\n", 1);
		free(data);
	}
}

void	execute_child_process(char *temp_file)
{
	int	temp_fd;

	temp_fd = open(temp_file, O_RDONLY);
	if (temp_fd == -1)
	{
		perror("open");
		exit(1);
	}
	dup2(temp_fd, STDIN_FILENO);
	close(temp_fd);
	execlp("cat", "cat", NULL);
	perror("execlp");
	exit(1);
}

void	handle_process(char *temp_file)
{
	pid_t	pid;

	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		exit(1);
	}
	if (pid == 0)
		execute_child_process(temp_file);
	else
		wait(NULL);
}

void	here_doc(char *limiter)
{
	char	temp_file[64];
	int		temp_fd;

	create_temp_file(temp_file);
	temp_fd = open_temp_file(temp_file);
	write_to_temp_file(temp_fd, limiter);
	close(temp_fd);
	handle_process(temp_file);
	unlink(temp_file);
}
