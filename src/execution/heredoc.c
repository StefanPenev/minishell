/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anilchen <anilchen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/18 17:05:03 by anilchen          #+#    #+#             */
/*   Updated: 2024/11/21 15:35:56 by anilchen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// #include "process.h"

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

// int	minishell_output_redirection(char *cmd_token, char *filename, t_env env_copy,
// 		t_process *process)
// {
// 	int	newfd;
// 	int	oldfd;

// 	if (ft_strcmp(cmd_token, ">") == 0)
// 	{
// 		newfd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
// 	}
// 	else if (ft_strcmp(cmd_token, ">>") == 0)
// 	{
// 		newfd = open(filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
// 	}
//     //if no permissions
// 	if (newfd == -1)
// 	{
// 		write(2, "minishell: permission denied: ", 30);
// 		write(2, &filename, ft_strlen(filename));
// 		write(2, "\n", 1);
// 		set_exit_status(process, 1);
// 		return (EXIT_FAILURE);
// 	}
//     //save old stdoit
// 	dup(oldfd);
//     //redirect new stdoit
// 	dup2(newfd, STDOUT_FILENO);
// 	close(newfd);
// 	// ...
//     // restore old stdoit
// 	dup2(oldfd, STDOUT_FILENO);
// 	close(oldfd);
// }

// // int	minishell_redirection(char *cmd_token, char *filename, t_env env_copy,
// // 		t_process *process)
// // {
// //     if
// // 		if (splitted[2] == ">" || splitted[2] == ">>" )
// // }