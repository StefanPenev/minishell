/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit_status.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anilchen <anilchen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 15:25:48 by anilchen          #+#    #+#             */
/*   Updated: 2024/12/04 15:04:37 by anilchen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "process.h"

// Updates the last exit status in the process structure.

void	set_exit_status(t_process *process, int status)
{
	process->last_exit_status = status;
}
