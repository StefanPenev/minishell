/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes_commands.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anilchen <anilchen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 14:43:02 by anilchen          #+#    #+#             */
/*   Updated: 2024/12/02 16:31:18 by anilchen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "process.h"

// Executes the first command in a pipeline.
// Configures the file descriptors for the pipeline, handles built-in commands,
// and executes external commands.
// Exits with the appropriate status if an error occurs.

void	first_cmd(t_pipe_fds *fds, t_command *cmd, char **env_array,
		t_shell_context *shell_ctx)
{
	char	*full_path;

	handle_streams(fds, cmd, PIPE_FIRST);
	if (is_builtin(cmd))
	{
		execute_builtin(cmd, shell_ctx->env_copy, shell_ctx->process);
		exit(shell_ctx->process->last_exit_status);
	}
	if (is_executable(cmd->args[0]))
		full_path = cmd->args[0];
	else
	{
		full_path = find_full_path(cmd->args[0], env_array);
		if (!full_path)
		{
			printf("minishell: %s: No such file or directory\n", cmd->args[0]);
			exit(127);
		}
	}
	execve(full_path, cmd->args, env_array);
	printf("minishell: %s: %s\n", cmd->args[0], strerror(errno));
	exit(EXIT_FAILURE);
}

// Executes a command in the middle of a pipeline.
// Configures the file descriptors for the pipeline, handles built-in commands,
// and executes external commands.
// Exits with the appropriate status if an error occurs.

void	execute_middle_cmd(t_pipe_fds *fds, t_command *cmd,
		t_pipes_process_content *ctx)
{
	char	*full_path;

	handle_streams(fds, cmd, PIPE_MIDDLE);
	if (is_builtin(cmd))
	{
		execute_builtin(cmd, ctx->shell_ctx->env_copy, ctx->shell_ctx->process);
		exit(ctx->shell_ctx->process->last_exit_status);
	}
	if (is_executable(cmd->args[0]))
		full_path = cmd->args[0];
	else
	{
		full_path = find_full_path(cmd->args[0], ctx->env_array);
		if (!full_path)
		{
			printf("minishell: %s: No such file or directory\n", cmd->args[0]);
			exit(127);
		}
	}
	execve(full_path, cmd->args, ctx->env_array);
	printf("minishell: %s: %s\n", cmd->args[0], strerror(errno));
	exit(EXIT_FAILURE);
}

// Creates a process for a middle command in a pipeline.
// Sets up the necessary pipes, forks a new process,
//	and executes the middle command.

int	process_middle_cmd(t_pipe_fds *fds, pid_t *pid, t_command *cmd,
		t_pipes_process_content *ctx)
{
	fds->fd_prev[0] = fds->fd[0];
	fds->fd_prev[1] = fds->fd[1];
	if (pipe(ctx->fds.fd) == -1)
	{
		ft_putstr_fd("Pipe failed\n", 2);
		set_exit_status(ctx->shell_ctx->process, 1);
		free_splitted(ctx->env_array);
		return (EXIT_FAILURE);
	}
	*pid = fork();
	if (*pid == -1)
	{
		ft_putstr_fd("Fork failed: Unable to create new process\n", 2);
		return (-1);
	}
	if (*pid == 0)
	{
		execute_middle_cmd(&ctx->fds, cmd, ctx);
		exit(EXIT_FAILURE);
	}
	close_safe(fds->fd_prev[0]);
	close_safe(fds->fd_prev[1]);
	return (EXIT_SUCCESS);
}

// Executes the last command in a pipeline.
// Configures the file descriptors for the pipeline,
// handles built-in commands, and executes external commands.

void	last_cmd(t_pipe_fds *fds, t_command *cmd, char **env_array,
		t_shell_context *shell_ctx)
{
	char	*full_path;

	handle_streams(fds, cmd, PIPE_LAST);
	if (is_builtin(cmd))
	{
		execute_builtin(cmd, shell_ctx->env_copy, shell_ctx->process);
		exit(shell_ctx->process->last_exit_status);
	}
	if (is_executable(cmd->args[0]))
		full_path = cmd->args[0];
	else
	{
		full_path = find_full_path(cmd->args[0], env_array);
		if (!full_path)
		{
			printf("minishell: %s: No such file or directory\n", cmd->args[0]);
			exit(127);
		}
	}
	execve(full_path, cmd->args, env_array);
	printf("minishell: %s: %s\n", cmd->args[0], strerror(errno));
	exit(EXIT_FAILURE);
}

// This function processes all HEREDOC redirections in the abstract syntax
// tree (AST). It traverses the tree, and for each command node,
// it iterates through its redirections. If a HEREDOC is found,
// it calls the heredoc function to handle it. In case of nested pipelines,
// it recursively processes the left and right branches of the pipeline.
// Returns -1 if any HEREDOC processing fails, otherwise 0.

int	process_all_heredocs(t_ast *ast)
{
	t_command		*cmd;
	t_redirection	*redir;

	if (ast->type == AST_COMMAND)
	{
		cmd = ast->u_data.command;
		redir = cmd->redirections;
		while (redir)
		{
			if (redir->type == HEREDOC)
			{
				if (heredoc(redir) == -1)
					return (-1);
			}
			redir = redir->next;
		}
	}
	else if (ast->type == AST_PIPELINE)
	{
		if (process_all_heredocs(ast->u_data.pipeline.left) == -1)
			return (-1);
		if (process_all_heredocs(ast->u_data.pipeline.right) == -1)
			return (-1);
	}
	return (0);
}
