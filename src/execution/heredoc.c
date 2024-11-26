/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anilchen <anilchen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/18 17:05:03 by anilchen          #+#    #+#             */
/*   Updated: 2024/11/26 14:49:08 by anilchen         ###   ########.fr       */
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
