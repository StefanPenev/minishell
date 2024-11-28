/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anilchen <anilchen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 14:36:21 by anilchen          #+#    #+#             */
/*   Updated: 2024/11/28 19:57:19 by anilchen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "process.h"

// void	assign_fds_if_redirection(t_command *cmd)
// {
// 	t_redirection	*redir;
// 	int				fd_out;
// 	int				fd_append;
// 	int				fd_in;
// 	t_redirection	*last_input;
// 	t_redirection	*last_output;

// 	while (redir)
// 	{
// 		if (redir->type == REDIRECT_INPUT)
// 		{	
			
// 		 	printf("[DEBUG] Parsed redirection: type = %d, number: %d, file = %s\n", redir->type, count, redir->filename);
// 			last_input = redir; 
// 			count++; //Debug
// 		}
// 		if (redir->type == REDIRECT_OUTPUT || REDIRECT_APPEND)
// 		{
// 					 	printf("[DEBUG] Parsed redirection: type = %d, number: %d, file = %s\n", redir->type, count, redir->filename);
// 			last_input = redir; 
// 			count++; //Debug
// 		}
// 		redir = redir->next;
// 	}
// 	redir = cmd->redirections;
// 	while (redir)
// 	{
// 		if (redir->type == REDIRECT_OUTPUT)
// 		{
// 			fd_out = open(redir->filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
// 			if (fd_out < 0)
// 			{
// 				perror("Error opening file for output redirection");
// 				exit(EXIT_FAILURE);
// 			}
// 			dup2(fd_out, STDOUT_FILENO); // Дублируем в STDOUT
// 			close_safe(fd_out);          // Закрываем дескриптор
// 		}
// 		else if (redir->type == REDIRECT_APPEND)
// 		{
// 			fd_append = open(redir->filename, O_WRONLY | O_CREAT | O_APPEND,
// 					0644);
// 			if (fd_append < 0)
// 			{
// 				perror("Error opening file for append redirection");
// 				exit(EXIT_FAILURE);
// 			}
// 			dup2(fd_append, STDOUT_FILENO); // Дублируем в STDOUT
// 			close_safe(fd_append);          // Закрываем дескриптор
// 		}
// 		else if (redir->type == REDIRECT_INPUT)
// 		{
// 			fd_in = open(redir->filename, O_RDONLY);
// 			if (fd_in < 0)
// 			{
// 				perror("Error opening file for input redirection");
// 				exit(EXIT_FAILURE);
// 			}
// 			dup2(fd_in, STDIN_FILENO); // Дублируем в STDIN
// 			close_safe(fd_in);         // Закрываем дескриптор
// 		}
// 		redir = redir->next; // Переходим к следующему редиректу
// 	}
// }

bool	cmd_has_input_redirection(t_command *cmd)
{
	t_redirection	*redir;

	redir = cmd->redirections;
	while (redir)
	{
		if (redir->type == REDIRECT_INPUT)
			return (true);
		redir = redir->next;
	}
	return (false);
}

bool	cmd_has_output_redirection(t_command *cmd)
{
	t_redirection	*redir;

	redir = cmd->redirections;
	while (redir)
	{
		if (redir->type == REDIRECT_OUTPUT || redir->type == REDIRECT_APPEND)
			return (true);
		redir = redir->next;
	}
	return (false);
}
void	handle_streams(t_pipe_fds *fds, t_command *cmd, int flag)
{
	if (flag == PIPE_FIRST)
	{
		if (fds->fd[1] >= 0)
			dup_stream(fds->fd[1], STDOUT_FILENO);
		close_safe(fds->fd[0]);
		close_safe(fds->fd[1]);
	}
	else if (flag == PIPE_MIDDLE)
	{
		// if (cmd && cmd->redirections)
		// 	handle_redirections(cmd);
		if (!cmd_has_input_redirection(cmd) && fds->fd_prev[0] >= 0)
			dup_stream(fds->fd_prev[0], STDIN_FILENO);
		if (!cmd_has_output_redirection(cmd) && fds->fd[1] >= 0)
			dup_stream(fds->fd[1], STDOUT_FILENO);
		close_safe(fds->fd_prev[0]);
		close_safe(fds->fd_prev[1]);
		close_safe(fds->fd[0]);
		close_safe(fds->fd[1]);
	}
	else if (flag == PIPE_LAST)
	{
		if (fds->fd[0] >= 0)
			dup_stream(fds->fd[0], STDIN_FILENO); // Используем пайп как вход
		close_safe(fds->fd[0]);
		close_safe(fds->fd[1]);
	}
	// Обрабатываем редиректы после настройки пайпов
	if (cmd && cmd->redirections)
	{
		if (handle_redirections(cmd) == -1)
		{
			fprintf(stderr, "Error handling redirections\n");
			exit(EXIT_FAILURE);
		}
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

void	collect_commands_in_order(t_ast *ast, t_command *cmds[], int *count)
{
	if (ast == NULL)
		return ;
	if (ast->type == AST_COMMAND)
	{
		cmds[*count] = ast->u_data.command;
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
	assign_pid(ctx);
	collect_commands_in_order(ast, ctx->cmds, &count);
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
