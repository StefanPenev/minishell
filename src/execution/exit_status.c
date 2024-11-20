/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit_status.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anilchen <anilchen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 15:25:48 by anilchen          #+#    #+#             */
/*   Updated: 2024/11/18 18:05:51 by anilchen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "process.h"

// void	call_exit_status(t_process *process)
// {
// 	char	*status_str;

// 	status_str = ft_itoa(process->last_exit_status);
// 	write(1, status_str, ft_strlen(status_str));
// 	free(status_str);
// }

void	set_exit_status(t_process *process, int status)
{
	process->last_exit_status = status;
}