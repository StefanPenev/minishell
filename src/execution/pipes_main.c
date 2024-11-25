/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes_main.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anilchen <anilchen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/21 15:08:15 by anilchen          #+#    #+#             */
/*   Updated: 2024/11/25 16:31:09 by anilchen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "process.h"

// ls -l | grep ".txt" | wc -l
// echo 123 | ls -l | wc -l
/// bin/ps aux | /bin/grep "bash" | /usr/bin/awk '{print $2}'

void	wait_for_children(t_pipes_process_content *ctx,
		t_shell_context *shell_ctx)
{
	int	i;

	i = 0;
	while (i < ctx->cmd_count)
	{
		handle_child_exit_status(ctx->pid[i], shell_ctx->process);
		i++;
	}
}

int	handle_last_command(t_pipes_process_content *ctx)
{
	int	last;

	last = ctx->cmd_count - 1;
	if (create_fork(&ctx->pid[last], ctx->shell_ctx->process) == -1)
	{
		close_safe(ctx->fds.fd[0]);
		close_safe(ctx->fds.fd[1]);
		return (EXIT_FAILURE);
	}
	if (ctx->pid[last] == 0)
	{
		last_cmd(&ctx->fds, ctx->cmds[last], ctx->env_array, ctx->shell_ctx);
	}
	close_safe(ctx->fds.fd[0]);
	close_safe(ctx->fds.fd[1]);
	return (EXIT_SUCCESS);
}

int	handle_middle_commands(t_pipes_process_content *ctx)
{
	int	i;

	i = 1;
	while (i < ctx->cmd_count - 1)
	{
		if (pipe(ctx->fds.fd) == -1)
		{
			ft_putstr_fd("Pipe failed\n", 2);
			set_exit_status(ctx->shell_ctx->process, 1);
			free(ctx->pid);
			free_splitted(ctx->env_array);
			return (EXIT_FAILURE);
		}
		if (process_middle_cmd(&ctx->fds, ctx->pid[i], ctx->cmds[i], ctx) == -1)
		{
			close(ctx->fds.fd[0]);
			close(ctx->fds.fd[1]);
			free(ctx->pid);
			free_splitted(ctx->env_array);
			return (EXIT_FAILURE);
		}
		i++;
	}
	return (EXIT_SUCCESS);
}

int	handle_first_command(t_pipes_process_content *ctx)
{
	if (pipe(ctx->fds.fd) == -1)
	{
		ft_putstr_fd("Pipe failed\n", 2);
		set_exit_status(ctx->shell_ctx->process, 1);
		return (EXIT_FAILURE);
	}
	if (create_fork(&ctx->pid[0], ctx->shell_ctx->process) == -1)
	{
		close_safe(ctx->fds.fd[0]);
		close_safe(ctx->fds.fd[1]);
		return (EXIT_FAILURE);
	}
	if (ctx->pid[0] == 0)
	{
		first_cmd(&ctx->fds, ctx->cmds[0], ctx->env_array, ctx->shell_ctx);
	}
	return (EXIT_SUCCESS);
}

int	main_pipes_process(t_ast *ast, t_shell_context *shell_ctx)
{
	t_pipes_process_content	ctx;

	if (initialize_pipes_process(ast, shell_ctx, &ctx) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	if (handle_first_command(&ctx) == EXIT_FAILURE)
	{
		cleanup_pipes_process(&ctx);
		return (EXIT_FAILURE);
	}
	if (handle_middle_commands(&ctx) == EXIT_FAILURE)
	{
		cleanup_pipes_process(&ctx);
		return (EXIT_FAILURE);
	}
	if (handle_last_command(&ctx) == EXIT_FAILURE)
	{
		cleanup_pipes_process(&ctx);
		return (EXIT_FAILURE);
	}
	wait_for_children(&ctx, shell_ctx);
	cleanup_pipes_process(&ctx);
	return (EXIT_SUCCESS);
}
