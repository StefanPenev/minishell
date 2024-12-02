/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes_main.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anilchen <anilchen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/21 15:08:15 by anilchen          #+#    #+#             */
/*   Updated: 2024/12/02 16:03:47 by anilchen         ###   ########.fr       */
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
		cleanup_pipes_process(&ctx);
		return (EXIT_FAILURE);
	}
	wait_for_children(&ctx, shell_ctx);
	cleanup_pipes_process(&ctx);
	return (EXIT_SUCCESS);
}

// int	handle_ast_pipeline(t_ast *ast, t_pipes_process_content *ctx)
// {
// 	t_ast		*current;
// 	static int	index;
// 	t_command	*left_cmd;
// 	t_command	*right_cmd;

// 	current = ast;
// 	index = 0;
// 	if (!current)
// 	{
// 		fprintf(stderr, "[ERROR] AST is NULL.\n");
// 		return (EXIT_FAILURE);
// 	}
// 	while (current)
// 	{
// 		if (current->type == AST_PIPELINE)
// 		{
// 			if (current->u_data.pipeline.left)
// 			{
// 				if (current->u_data.pipeline.left->type == AST_COMMAND)
// 				{
// 					left_cmd = current->u_data.pipeline.left->u_data.command;
// 					// if (execute_left_command(index, left_cmd, current,
// 					// 		ctx) == EXIT_FAILURE)
// 					// 	return (EXIT_FAILURE);
// 					if (index == 0)
// 					{
// 						if (handle_first_command(current->u_data.pipeline.left,
// 								ctx) == EXIT_FAILURE)
// 						{
// 							// printf("[ERROR] Ошибка обработки первой команды
// 							// 	%s\n", left_cmd->args[0]);
// 							return (EXIT_FAILURE);
// 						}
// 					}
// 					else
// 					{
// 				if (handle_middle_commands(current->u_data.pipeline.left,
// 								ctx, index) == EXIT_FAILURE)
// 						{
// 				// printf("[ERROR] Ошибка обработки промежуточной команды
// 							// 	%s\n", left_cmd->args[0]);
// 							return (EXIT_FAILURE);
// 						}
// 					}
// 					index++;
// 				}
// 				else if (current->u_data.pipeline.left->type == AST_PIPELINE)
// 				{
// 					handle_ast_pipeline(current->u_data.pipeline.left, ctx);
// 				}
// 				else
// 				{
// 	// printf("[ERROR] Неподдерживаемый тип в левой части пайплайна:% d(индекс:
// 					// 	% d).\n ",
// 					// 				current->u_data.pipeline.left->type, index);
// 					return (EXIT_FAILURE);
// 				}
// 			}
// 		}
// 		if (current->type == AST_PIPELINE && current->u_data.pipeline.right
// 			&& current->u_data.pipeline.right->type == AST_COMMAND)
// 		{
// 			right_cmd = current->u_data.pipeline.right->u_data.command;
// 			if (index < ctx->cmd_count - 1)
// 			{
// 				if (handle_middle_commands(current->u_data.pipeline.right, ctx,
// 						index) == EXIT_FAILURE)
// 				{
// 					// fprintf(stderr,
// 					// 	"[ERROR] Ошибка обработки промежуточной команды:
// 					// 	%s\n", right_cmd->args[0]);
// 					return (EXIT_FAILURE);
// 				}
// 			}
// 			else
// 			{
// 				if (handle_last_command(current->u_data.pipeline.right,
// 						ctx) == EXIT_FAILURE)
// 				{
// 					// fprintf(stderr,
// 					// 	"[ERROR] Ошибка обработки последней команды:
// 					// // 	%s\n", right_cmd->args[0]);
// 					return (EXIT_FAILURE);
// 				}
// 				break ;
// 			}
// 			index++;
// 		}
// 		if (current->type == AST_PIPELINE)
// 		{
// 			current = current->u_data.pipeline.right;
// 		}
// 		else
// 		{
// 			current = NULL;
// 		}
// 	}
// 	return (EXIT_SUCCESS);
// }
