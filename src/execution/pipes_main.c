/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes_main.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anilchen <anilchen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/21 15:08:15 by anilchen          #+#    #+#             */
/*   Updated: 2024/11/28 14:41:52 by anilchen         ###   ########.fr       */
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

int	handle_first_command(t_ast *ast, t_pipes_process_content *ctx)
{
	t_command	*cmd;
	//int status;

	cmd = ast->u_data.command;
	if (pipe(ctx->fds.fd) == -1)
	{
		ft_putstr_fd("Pipe failed\n", 2);
		set_exit_status(ctx->shell_ctx->process, 1);
		return (EXIT_FAILURE);
	}
	//printf("[DEBUG] first cmd - pipe created\n");
		ctx->pid[0] = fork();
	if (ctx->pid[0] == -1)
		return EXIT_FAILURE;
	// if (create_fork(&ctx->pid[0], ctx->shell_ctx->process) == -1)
	// {
	// 	close_safe(ctx->fds.fd[0]);
	// 	close_safe(ctx->fds.fd[1]);
	// 	return (EXIT_FAILURE);
	// }
	if (ctx->pid[0] == 0)
	{
		//printf("[DEBUG] Дочерний процесс создан для first команды: %s\n", cmd->args[0]);
		first_cmd(&ctx->fds, cmd, ctx->env_array, ctx->shell_ctx);
		exit(EXIT_FAILURE); 
	}
	// else
	// {
	// 	// Родительский процесс
	// 	waitpid(ctx->pid[0], &status, 0);
	// 	if (WIFEXITED(status))
	// 	{
	// 		int exit_code = WEXITSTATUS(status);
	// 		if (exit_code != 0)
	// 		{
	// 			ctx->shell_ctx->process->last_exit_status = exit_code;
	// 			fprintf(stderr, "[ERROR] Первая команда завершилась с ошибкой: %s\n", cmd->args[0]);
	// 			return (EXIT_FAILURE);
	// 		}
	// 	}
	// }
	return (EXIT_SUCCESS);
}

int	handle_middle_commands(t_ast *ast, t_pipes_process_content *ctx, int index)
{
	t_command	*cmd;

	if (ast->type == AST_COMMAND)
	{
		cmd = ast->u_data.command;
		if (process_middle_cmd(&ctx->fds, &ctx->pid[index], cmd, ctx) == -1)
		{
			close(ctx->fds.fd[0]);
			close(ctx->fds.fd[1]);
			free(ctx->pid);
			free_splitted(ctx->env_array);
			return (EXIT_FAILURE);
		}

	}
	return (EXIT_SUCCESS);
}

int	handle_last_command(t_ast *ast, t_pipes_process_content *ctx)
{
	t_command	*cmd;
	//int status = 0;

	cmd = ast->u_data.command;
	ctx->pid[ctx->cmd_count - 1] = fork();
	if (ctx->pid[ctx->cmd_count - 1] == -1)
		return (EXIT_FAILURE);
	if (ctx->pid[ctx->cmd_count - 1] == 0)
	{
		last_cmd(&ctx->fds, cmd, ctx->env_array, ctx->shell_ctx);
		exit(EXIT_FAILURE); 
	}
	// 	else
	// {
	// 	// Родительский процесс
	// 	waitpid(ctx->pid[ctx->cmd_count - 1], &status, 0);
	// 	if (WIFEXITED(status))
	// 	{
	// 		int exit_code = WEXITSTATUS(status);
	// 		if (exit_code != 0)
	// 		{
	// 			close_safe(ctx->fds.fd[0]);
	// 			close_safe(ctx->fds.fd[1]);
	// 			ctx->shell_ctx->process->last_exit_status = exit_code;
	// 			fprintf(stderr, "[ERROR] Первая команда завершилась с ошибкой: %s\n", cmd->args[0]);
	// 			return (EXIT_FAILURE);
	// 		}
	// 	}
	// }
	close_safe(ctx->fds.fd[0]);
	close_safe(ctx->fds.fd[1]);
	return (EXIT_SUCCESS);
}

int	handle_ast_pipeline(t_ast *ast, t_pipes_process_content *ctx)
{
	t_ast		*current;
	static int			index;
	t_command	*left_cmd;
	t_command	*right_cmd;

	current = ast;
	index = 0;
	if (!current)
	{
		fprintf(stderr, "[ERROR] AST is NULL.\n");
		return (EXIT_FAILURE);
	}
	// Итеративный обход узлов пайплайна
	while (current)
	{
		//printf("[DEBUG] Обрабатываем узел пайплайна (индекс: %d).\n", index);
		// Обрабатываем левую часть
		if (current->type == AST_PIPELINE)
		{
			if (current->u_data.pipeline.left)
			{
				if (current->u_data.pipeline.left->type == AST_COMMAND)
				{
					left_cmd = current->u_data.pipeline.left->u_data.command;
					// printf("[DEBUG] Left Command: %s (индекс: %d).\n",
					// 	left_cmd->args[0], index);
					if (index == 0)
					{
						//printf("[DEBUG] Обрабатываем первую команду %s\n", left_cmd->args[0]);
						if (handle_first_command(current->u_data.pipeline.left,
								ctx) == EXIT_FAILURE)
						{
							fprintf(stderr, "[ERROR] Ошибка обработки первой команды %s\n", left_cmd->args[0]);
							return (EXIT_FAILURE);
						}
					}
					else
					{
						//printf("[DEBUG] Обрабатываем промежуточную команду %s\n", left_cmd->args[0]);
						if (handle_middle_commands(current->u_data.pipeline.left,
								ctx, index) == EXIT_FAILURE)
						{
							fprintf(stderr, "[ERROR] Ошибка обработки промежуточной команды %s\n", left_cmd->args[0]);
							return (EXIT_FAILURE);
						}
					}
					index++;
				}
				else if (current->u_data.pipeline.left->type == AST_PIPELINE)
				{
					//printf("[DEBUG] Вложенный пайплайн в левой части (индекс: %d).\n", index);
					handle_ast_pipeline(current->u_data.pipeline.left, ctx);
				}
				else
				{
					fprintf(stderr, "[ERROR] Неподдерживаемый тип в левой части пайплайна: %d (индекс: %d).\n",
						current->u_data.pipeline.left->type, index);
					return (EXIT_FAILURE);
				}
			}
		}
		// Проверяем, является ли правая часть последней командой
		if (current->type == AST_PIPELINE && current->u_data.pipeline.right
			&& current->u_data.pipeline.right->type == AST_COMMAND)
		{
			right_cmd = current->u_data.pipeline.right->u_data.command;
			//printf("[DEBUG] Right Command: %s (индекс: %d).\n",
			//	right_cmd->args[0], index);
			if (index < ctx->cmd_count - 1)
			{
				// Промежуточная команда
				//printf("[DEBUG] Обрабатываем промежуточную команду: %s\n", right_cmd->args[0]);
				if (handle_middle_commands(current->u_data.pipeline.right, ctx,
						index) == EXIT_FAILURE)
				{
					fprintf(stderr,
						"[ERROR] Ошибка обработки промежуточной команды: %s\n", right_cmd->args[0]);
					return (EXIT_FAILURE);
				}
			}
			else
			{
				// Последняя команда
				//printf("[DEBUG] Обрабатываем последнюю команду: %s\n", right_cmd->args[0]);
				if (handle_last_command(current->u_data.pipeline.right,
						ctx) == EXIT_FAILURE)
				{
					fprintf(stderr, "[ERROR] Ошибка обработки последней команды: %s\n", right_cmd->args[0]);
					return (EXIT_FAILURE);
				}
				break ; // Завершаем цикл, так как это последняя команда
			}
			index++;
		}
		// Переходим к следующему узлу пайплайна
		if (current->type == AST_PIPELINE)
		{
			current = current->u_data.pipeline.right;
		}
		else
		{
			current = NULL;
		}
	}
	//index = 0;
	return (EXIT_SUCCESS);
}

int	main_pipes_process(t_ast *ast, t_shell_context *shell_ctx)
{
	t_pipes_process_content	ctx;

	// Инициализация пайплайнов
	if (initialize_pipes_process(ast, shell_ctx, &ctx) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	// Обработка узлов AST
	if (handle_ast_pipeline(ast, &ctx) == EXIT_FAILURE)
	{
		cleanup_pipes_process(&ctx);
		return (EXIT_FAILURE);
	}
// 	for (int i = 0; i < ctx.cmd_count; i++) {
//     printf("[DEBUG] PID[%d] перед ожиданием: %d\n", i, ctx.pid[i]);
// }

	wait_for_children(&ctx, shell_ctx);
	cleanup_pipes_process(&ctx);
	return (EXIT_SUCCESS);
}

// int	main_pipes_process(t_ast *ast, t_shell_context *shell_ctx)
// {
// 	t_pipes_process_content	ctx;

// 	if (initialize_pipes_process(ast, shell_ctx, &ctx) == EXIT_FAILURE)
// 		return (EXIT_FAILURE);
// 	if (handle_first_command(&ctx) == EXIT_FAILURE)
// 	{
// 		cleanup_pipes_process(&ctx);
// 		return (EXIT_FAILURE);
// 	}
// 	if (handle_middle_commands(&ctx) == EXIT_FAILURE)
// 	{
// 		cleanup_pipes_process(&ctx);
// 		return (EXIT_FAILURE);
// 	}
// 	if (handle_last_command(&ctx) == EXIT_FAILURE)
// 	{
// 		cleanup_pipes_process(&ctx);
// 		return (EXIT_FAILURE);
// 	}
// 	wait_for_children(&ctx, shell_ctx);
// 	cleanup_pipes_process(&ctx);
// 	return (EXIT_SUCCESS);
// }
