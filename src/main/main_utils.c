/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stfn <stfn@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/19 18:59:05 by stfn              #+#    #+#             */
/*   Updated: 2024/12/04 23:13:57 by stfn             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	assign_vars(t_shell_context **shell_ctx, t_process *process,
		t_env **env_copy, char **envp)
{
	*shell_ctx = malloc(sizeof(t_shell_context));
	if (!*shell_ctx)
	{
		printf("malloc");
		return ;
	}
	process->last_exit_status = 0;
	*env_copy = init_env(envp);
	(*shell_ctx)->process = process;
	(*shell_ctx)->env_copy = *env_copy;
}

int	is_builtin(t_command *cmd)
{
	static const char	*builtins[] = {"echo", "cd", "pwd", "export", "unset",
		"env", "exit", "/usr/bin/echo", "/bin/pwd", "/usr/bin/env",
		"/bin/echo"};
	size_t				i;

	i = 0;
	if (cmd == NULL || cmd->args == NULL || cmd->args[0] == NULL)
	{
		printf("Error: Invalid command or arguments.\n");
		exit(EXIT_FAILURE);
	}
	while (i < sizeof(builtins) / sizeof(builtins[0]))
	{
		if (ft_strcmp(cmd->args[0], builtins[i]) == 0)
		{
			return (1);
		}
		i++;
	}
	return (0);
}

void	restore_standard_fds(t_fd_backup *fd_backup)
{
	if (fd_backup->saved_stdin != -1 && isatty(fd_backup->saved_stdin))
	{
		if (dup2(fd_backup->saved_stdin, STDIN_FILENO) == -1)
		{
			perror("Failed to restore stdin");
		}
		close(fd_backup->saved_stdin);
	}
	if (fd_backup->saved_stdout != -1 && isatty(fd_backup->saved_stdout))
	{
		if (dup2(fd_backup->saved_stdout, STDOUT_FILENO) == -1)
		{
			perror("Failed to restore stdout");
		}
		close(fd_backup->saved_stdout);
	}
	if (fd_backup->saved_stderr != -1 && isatty(fd_backup->saved_stderr))
	{
		if (dup2(fd_backup->saved_stderr, STDERR_FILENO) == -1)
		{
			perror("Failed to restore stderr");
		}
		close(fd_backup->saved_stderr);
	}
}

int	setup_redirections(t_command *cmd, t_fd_backup *fd_backup,
		t_process *process)
{
	if (cmd->redirections)
	{
		fd_backup->saved_stdin = dup(STDIN_FILENO);
		fd_backup->saved_stdout = dup(STDOUT_FILENO);
		fd_backup->saved_stderr = dup(STDERR_FILENO);
		if (fd_backup->saved_stdin == -1 || fd_backup->saved_stdout == -1
			|| fd_backup->saved_stderr == -1)
		{
			perror("dup");
			return (-1);
		}
		if (handle_redirections(cmd, process) == -1)
		{
			restore_standard_fds(fd_backup);
			return (-1);
		}
		return (1);
	}
	else
	{
		fd_backup->saved_stdin = -1;
		fd_backup->saved_stdout = -1;
		fd_backup->saved_stderr = -1;
		return (0);
	}
}
