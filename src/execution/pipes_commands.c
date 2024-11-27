/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes_commands.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anilchen <anilchen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 14:43:02 by anilchen          #+#    #+#             */
/*   Updated: 2024/11/27 19:04:55 by anilchen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "process.h"

void	first_cmd(t_pipe_fds *fds, t_command *cmd, char **env_array,
		t_shell_context *shell_ctx)
{
	char	*full_path;

	handle_streams(fds, cmd, PIPE_FIRST);
	if (is_builtin(cmd))
	{
		// handle_streams(fds, PIPE_FIRST);
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
	// handle_streams(fds, PIPE_FIRST);
	execve(full_path, cmd->args, env_array);
	printf("minishell: %s: %s\n", cmd->args[0], strerror(errno));
	exit(EXIT_FAILURE);
}

void	execute_middle_cmd(t_pipe_fds *fds, t_pipes_process_content *cmd_ctx,
		t_command *cmd)
{
	char	*full_path;

	handle_streams(fds, cmd, PIPE_MIDDLE);
	if (is_builtin(cmd))
	{
		// handle_streams(fds, PIPE_MIDDLE);
		execute_builtin(cmd, cmd_ctx->shell_ctx->env_copy,
			cmd_ctx->shell_ctx->process);
		exit(cmd_ctx->shell_ctx->process->last_exit_status);
	}
	if (is_executable(cmd->args[0]))
		full_path = cmd->args[0];
	else
	{
		full_path = find_full_path(cmd->args[0], cmd_ctx->env_array);
		if (!full_path)
		{
			printf("minishell: %s: No such file or directory\n", cmd->args[0]);
			exit(127);
		}
	}
	//	handle_streams(fds, PIPE_MIDDLE);
	execve(full_path, cmd->args, cmd_ctx->env_array);
	printf("minishell: %s: %s\n", cmd->args[0], strerror(errno));
	exit(EXIT_FAILURE);
}

int	process_middle_cmd(t_pipe_fds *fds, pid_t *pid, t_command *cmd,
		t_pipes_process_content *cmd_ctx)
{
	fds->fd_prev[0] = fds->fd[0];
	fds->fd_prev[1] = fds->fd[1];
	if (pipe(cmd_ctx->fds.fd) == -1)
	{
		ft_putstr_fd("Pipe failed\n", 2);
		set_exit_status(cmd_ctx->shell_ctx->process, 1);
		free_splitted(cmd_ctx->env_array);
		return (EXIT_FAILURE);
	}
	//printf("[DEBUG] middle cmd - pipe created\n");
	*pid = fork();
	if (*pid == -1)
		return (-1);
	if (*pid == 0)
	{
		// printf("[DEBUG] Дочерний процесс создан для middle команды: %s\n",
		// 	cmd->args[0]);
		execute_middle_cmd(&cmd_ctx->fds, cmd_ctx, cmd);
	}
	close_safe(fds->fd_prev[0]);
	close_safe(fds->fd_prev[1]);
	return (EXIT_SUCCESS);
}

void	last_cmd(t_pipe_fds *fds, t_command *cmd, char **env_array,
		t_shell_context *shell_ctx)
{
	char	*full_path;

	handle_streams(fds, cmd, PIPE_LAST);
	if (is_builtin(cmd))
	{
		// handle_streams(fds,PIPE_LAST);
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
	// handle_streams(fds, PIPE_LAST);
	execve(full_path, cmd->args, env_array);
	printf("minishell: %s: %s\n", cmd->args[0], strerror(errno));
	exit(EXIT_FAILURE);
}
