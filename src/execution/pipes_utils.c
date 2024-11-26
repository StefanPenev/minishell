/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anilchen <anilchen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 14:36:21 by anilchen          #+#    #+#             */
/*   Updated: 2024/11/26 15:40:26 by anilchen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "process.h"

void	handle_streams(t_pipe_fds *fds, int flag)
{
	if (flag == PIPE_FIRST)
	{
		dup_stream(fds->fd[1], STDOUT_FILENO);
		close_safe(fds->fd[0]);
		close_safe(fds->fd[1]);
	}
	else if (flag == PIPE_MIDDLE)
	{
		dup_stream(fds->fd_prev[0], STDIN_FILENO);
		dup_stream(fds->fd[1], STDOUT_FILENO);
		close_safe(fds->fd_prev[0]);
		close_safe(fds->fd_prev[1]);
		close_safe(fds->fd[0]);
		close_safe(fds->fd[1]);
	}
	else if (flag == PIPE_LAST)
	{
		dup_stream(fds->fd[0], STDIN_FILENO);
		close_safe(fds->fd[0]);
		close_safe(fds->fd[1]);
	}
}

int	assign_pid(t_pipes_process_content *ctx)
{
	int	i;

	i = 0;
	ctx->pid = malloc(sizeof(pid_t) * ctx->cmd_count);
	if (!ctx->pid)
	{
		perror("malloc");
		free_splitted(ctx->env_array);
		return (EXIT_FAILURE);
	}
	while (i < ctx->cmd_count)
	{
		ctx->pid[i] = -1;
		i++;
	}
	return (EXIT_SUCCESS);
}

int	initialize_pipes_process(t_ast *ast, t_shell_context *shell_ctx,
		t_pipes_process_content *ctx)
{
	ctx->redir_filename = NULL; 
	ctx->redir_flag = 0; 
	ctx->pid = NULL;
	ctx->cmd_count = 0;
	collect_commands_in_order(ast, ctx->cmds, &ctx->cmd_count);
	ctx->env_array = create_env_array(shell_ctx->env_copy);
	if (!ctx->env_array)
	{
		perror("malloc");
		return (EXIT_FAILURE);
	}
	ctx->shell_ctx = shell_ctx;
	ctx->fds.fd[0] = -1;
	ctx->fds.fd[1] = -1;
	ctx->fds.fd_prev[0] = -1;
	ctx->fds.fd_prev[1] = -1;
	assign_pid(ctx);
	return (EXIT_SUCCESS);
}
