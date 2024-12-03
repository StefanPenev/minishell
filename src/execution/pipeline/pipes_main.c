/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes_main.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anilchen <anilchen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/21 15:08:15 by anilchen          #+#    #+#             */
/*   Updated: 2024/12/03 17:01:45 by anilchen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "process.h"

// ls -l | grep ".txt" | wc -l
// echo 123 | ls -l | wc -l
/// bin/ps aux | /bin/grep "bash" | /usr/bin/awk '{print $2}'

// Waits for all child processes in the pipeline to complete.
// Iterates over all process IDs in the pipeline and handles their
// exit statuses.

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

// Handles the execution of the first command in the pipeline.
// Creates a pipe for inter-process communication and forks a new process to
// execute the command.
// Returns EXIT_FAILURE if pipe creation or fork fails, otherwise EXIT_SUCCESS.

int	handle_first_command(t_ast *ast, t_pipes_process_content *ctx)
{
	t_command	*cmd;

	cmd = ast->u_data.command;
	if (pipe(ctx->fds.fd) == -1)
	{
		ft_putstr_fd("Pipe failed\n", 2);
		set_exit_status(ctx->shell_ctx->process, 1);
		return (EXIT_FAILURE);
	}
	ctx->pid[0] = fork();
	if (ctx->pid[0] == -1)
	{
		ft_putstr_fd("Fork failed: Unable to create new process\n", 2);
		close_safe(ctx->fds.fd[0]);
		close_safe(ctx->fds.fd[1]);
		return (EXIT_FAILURE);
	}
	if (ctx->pid[0] == 0)
	{
		first_cmd(&ctx->fds, cmd, ctx->env_array, ctx->shell_ctx);
		exit(EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}

// Handles the execution of middle commands in the pipeline.
// Processes a command in the middle of the pipeline by setting up pipes
// and forking a process.
// Returns EXIT_FAILURE if the process creation fails, otherwise EXIT_SUCCESS.

int	handle_middle_commands(t_ast *ast, t_pipes_process_content *ctx, int index)
{
	t_command	*cmd;

	cmd = ast->u_data.command;
	if (process_middle_cmd(&ctx->fds, &ctx->pid[index], cmd, ctx) == -1)
	{
		close_safe(ctx->fds.fd[0]);
		close_safe(ctx->fds.fd[1]);
		return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}

// Handles the execution of the last command in the pipeline.
// Forks a new process to execute the last command and closes unused pipe ends.
// Returns EXIT_FAILURE if fork fails, otherwise EXIT_SUCCESS.

int	handle_last_command(t_ast *ast, t_pipes_process_content *ctx)
{
	t_command	*cmd;

	cmd = ast->u_data.command;
	ctx->pid[ctx->cmd_count - 1] = fork();
	if (ctx->pid[ctx->cmd_count - 1] == -1)
		return (EXIT_FAILURE);
	if (ctx->pid[ctx->cmd_count - 1] == 0)
	{
		last_cmd(&ctx->fds, cmd, ctx->env_array, ctx->shell_ctx);
		exit(EXIT_FAILURE);
	}
	close_safe(ctx->fds.fd[0]);
	close_safe(ctx->fds.fd[1]);
	return (EXIT_SUCCESS);
}

// Main function to process a pipeline of commands.
// Initializes the pipeline context, processes the AST pipeline, waits for
// child processes, and cleans up resources.
// Returns EXIT_FAILURE if any step fails, otherwise EXIT_SUCCESS.

int	main_pipes_process(t_ast *ast, t_shell_context *shell_ctx)
{
	t_pipes_process_content	ctx;

	if (process_all_heredocs(ast) == -1)
		return (EXIT_FAILURE);
	if (initialize_pipes_process(ast, shell_ctx, &ctx) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	if (handle_ast_pipeline(ast, &ctx) == EXIT_FAILURE)
	{
		cleanup_pipes(&ctx);
		return (EXIT_FAILURE);
	}
	wait_for_children(&ctx, shell_ctx);
	cleanup_pipes(&ctx);
	return (EXIT_SUCCESS);
}
