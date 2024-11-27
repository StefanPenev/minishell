/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anilchen <anilchen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 14:36:21 by anilchen          #+#    #+#             */
/*   Updated: 2024/11/27 19:14:44 by anilchen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "process.h"
#include <stdio.h> // Для printf

void	handle_streams(t_pipe_fds *fds, t_command *cmd, int flag)
{
	// Отладочное сообщение перед обработкой редиректов
	// printf("[DEBUG] Handling redirections and streams. Flag: %d\n", flag);
	// printf("[DEBUG] Initial fds: fd[0]: %d, fd[1]: %d, fd_prev[0]: %d,
	//	fd_prev[1]: %d\n",
	//        fds->fd[0], fds->fd[1], fds->fd_prev[0], fds->fd_prev[1]);
	if (flag == PIPE_FIRST)
	{
		printf("[DEBUG] PIPE_FIRST: Redirecting stdout to fd[1]: %d\n",
			fds->fd[1]);
		if (fds->fd[1] >= 0)
			dup_stream(fds->fd[1], STDOUT_FILENO);
		close_safe(fds->fd[0]);
		close_safe(fds->fd[1]);
	}
	else if (flag == PIPE_MIDDLE)
	{
		printf("[DEBUG] PIPE_MIDDLE: Redirecting stdin from fd_prev[0]: %d, stdout to fd[1]: %d\n", fds->fd_prev[0], fds->fd[1]);
		if (fds->fd_prev[0] >= 0)
			dup_stream(fds->fd_prev[0], STDIN_FILENO);
		if (fds->fd[1] >= 0)
			dup_stream(fds->fd[1], STDOUT_FILENO);
		close_safe(fds->fd_prev[0]);
		close_safe(fds->fd_prev[1]);
		close_safe(fds->fd[0]);
		close_safe(fds->fd[1]);
	}
	else if (flag == PIPE_LAST)
	{
		printf("[DEBUG] PIPE_LAST: Redirecting stdin from fd[0]: %d\n",
			fds->fd[0]);
		if (fds->fd[0] >= 0)
			dup_stream(fds->fd[0], STDIN_FILENO);
		close_safe(fds->fd[0]);
		close_safe(fds->fd[1]);
	}
	// Обработка редиректов
	if (cmd && cmd->redirections)
	{
		printf("[DEBUG] Applying redirections for command: %s\n",cmd->args[0]);
		if (handle_redirections(cmd) == -1)
		{
			fprintf(stderr, "Error handling redirections\n");
			exit(EXIT_FAILURE);
		}
	}
	// Отладочное сообщение после обработки
	// printf("[DEBUG] Final fds: fd[0]: %d, fd[1]: %d, fd_prev[0]: %d,
	//	fd_prev[1]: %d\n",
	//        fds->fd[0], fds->fd[1], fds->fd_prev[0], fds->fd_prev[1]);
}


// void	handle_streams(t_pipe_fds *fds, int flag)
// {
// 	if (flag == PIPE_FIRST)
// 	{
// 		dup_stream(fds->fd[1], STDOUT_FILENO);
// 		close_safe(fds->fd[0]);
// 		close_safe(fds->fd[1]);
// 	}
// 	else if (flag == PIPE_MIDDLE)
// 	{
// 		dup_stream(fds->fd_prev[0], STDIN_FILENO);
// 		dup_stream(fds->fd[1], STDOUT_FILENO);
// 		close_safe(fds->fd_prev[0]);
// 		close_safe(fds->fd_prev[1]);
// 		close_safe(fds->fd[0]);
// 		close_safe(fds->fd[1]);
// 	}
// 	else if (flag == PIPE_LAST)
// 	{
// 		dup_stream(fds->fd[0], STDIN_FILENO);
// 		close_safe(fds->fd[0]);
// 		close_safe(fds->fd[1]);
// 	}
// }

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
		// printf("[DEBUG]: pid N %d assigned\n", i);
		i++;
	}
	return (EXIT_SUCCESS);
}

void	collect_commands_in_order(t_ast *ast, t_command *cmds[], int *count)
{
	if (ast == NULL)
		return ;
	if (ast->type == AST_COMMAND)
	{
		cmds[*count] = ast->u_data.command;
		// printf("[DEBUG] Команда %d: %s\n", *count, cmds[*count]->args[0]);
		(*count)++;
	}
	else if (ast->type == AST_PIPELINE)
	{
		collect_commands_in_order(ast->u_data.pipeline.left, cmds, count);
		collect_commands_in_order(ast->u_data.pipeline.right, cmds, count);
	}
}

int	count_commands_in_ast(t_ast *ast)
{
	if (!ast)
		return (0);
	if (ast->type == AST_COMMAND)
		return (1);
	if (ast->type == AST_PIPELINE)
		return (count_commands_in_ast(ast->u_data.pipeline.left)
			+ count_commands_in_ast(ast->u_data.pipeline.right));
	return (0);
}

int	initialize_pipes_process(t_ast *ast, t_shell_context *shell_ctx,
		t_pipes_process_content *ctx)
{
	int	count;

	count = 0;
	ctx->pid = NULL;
	ctx->cmd_count = count_commands_in_ast(ast);
	// printf("%d\n", ctx->cmd_count);
	assign_pid(ctx);
	collect_commands_in_order(ast, ctx->cmds, &count);
	// 	for (int i = 0; i < ctx->cmd_count; i++)
	// {
	//     printf("[DEBUG] Команда %d после collect_commands_in_order: %s\n", i,
	//	ctx->cmds[i]->args[0]);
	// }
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
	return (EXIT_SUCCESS);
}

// int	initialize_pipes_process(t_ast *ast, t_shell_context *shell_ctx,
// 		t_pipes_process_content *ctx)
// {
// 	ctx->pid = NULL;
// 	ctx->cmd_count = 0;
// 	collect_commands_in_order(ast, ctx->cmds, &ctx->cmd_count);
// 	ctx->env_array = create_env_array(shell_ctx->env_copy);
// 	if (!ctx->env_array)
// 	{
// 		perror("malloc");
// 		return (EXIT_FAILURE);
// 	}
// 	ctx->shell_ctx = shell_ctx;
// 	ctx->fds.fd[0] = -1;
// 	ctx->fds.fd[1] = -1;
// 	ctx->fds.fd_prev[0] = -1;
// 	ctx->fds.fd_prev[1] = -1;
// 	assign_pid(ctx);
// 	return (EXIT_SUCCESS);
// }
