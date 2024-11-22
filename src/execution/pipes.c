/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anilchen <anilchen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/21 15:08:15 by anilchen          #+#    #+#             */
/*   Updated: 2024/11/22 17:13:58 by anilchen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "process.h"

// ls -l | grep ".txt" | wc -l

int	create_fork(pid_t *pid, t_process *process)
{
	*pid = fork();
	if (*pid == -1)
	{
		ft_putstr_fd("Fork failed\n", 2);
		set_exit_status(process, 1);
		return (-1);
	}
	return (EXIT_SUCCESS);
}

void	if_is_builtin(t_pipe_fds *fds, t_command *cmd,
		t_shell_context *shell_ctx, int flag)
{
	if (flag == 1)
	{
		dup2(fds->fd_prev[0], STDIN_FILENO);
		close(fds->fd_prev[0]);
		close(fds->fd_prev[1]);
	}
	if (flag != 2)
	{
		dup2(fds->fd_prev[0], STDIN_FILENO);
		dup2(fds->fd[1], STDOUT_FILENO);
		close(fds->fd[0]);
		close(fds->fd[1]);
		close(fds->fd_prev[0]);
		close(fds->fd_prev[1]);
	}
	if (flag == 3)
	{
		dup2(fds->fd[1], STDOUT_FILENO);
		close(fds->fd[0]);
		close(fds->fd[1]);
	}
	execute_builtin(cmd, shell_ctx->env_copy, shell_ctx->process); //, 1);
	exit(shell_ctx->process->last_exit_status);
}

void	first_cmd(t_pipe_fds *fds, t_command *cmd, char **env_array,
		t_shell_context *shell_ctx)
{
	char	*full_path;

	if (is_builtin(cmd))
		if_is_builtin(fds, cmd, shell_ctx, 1);
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
	dup2(fds->fd[1], STDOUT_FILENO);
	close(fds->fd[0]);
	close(fds->fd[1]);
	execve(full_path, cmd->args, env_array);
	printf("minishell: %s: %s\n", cmd->args[0], strerror(errno));
	exit(EXIT_FAILURE);
}

void	execute_middle_cmd(t_pipe_fds *fds, t_command *cmd, char **env_array,
		t_shell_context *shell_ctx)
{
	char	*full_path;

	if (is_builtin(cmd))
		if_is_builtin(fds, cmd, shell_ctx, 2);
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
	if (dup2(fds->fd_prev[0], STDIN_FILENO) == -1)
	{
		perror("dup2 failed");
		exit(EXIT_FAILURE);
	}
	if (dup2(fds->fd[1], STDOUT_FILENO) == -1)
	{
		perror("dup2 failed");
		exit(EXIT_FAILURE);
	}
	close(fds->fd_prev[0]);
	close(fds->fd_prev[1]);
	close(fds->fd[0]);
	close(fds->fd[1]);
	execve(full_path, cmd->args, env_array);
	// If execve fails
	printf("minishell: %s: %s\n", cmd->args[0], strerror(errno));
	exit(1);
}

int	process_middle_cmd(pid_t pid, t_pipe_fds *fds, t_command *cmd,
		char **env_array, t_shell_context *shell_ctx)
{
	// pid_t	pid;
	fds->fd_prev[0] = fds->fd[0];
	fds->fd_prev[1] = fds->fd[1];
	if (pipe(fds->fd) == -1)
	{
		ft_putstr_fd("Pipe failed\n", 2);
		set_exit_status(shell_ctx->process, 1);
		free_splitted(env_array);
		return (EXIT_FAILURE);
	}
	if (create_fork(&pid, shell_ctx->process) == -1)
		return (-1);
	if (pid == 0)
	{
		execute_middle_cmd(fds, cmd, env_array, shell_ctx);
	}
	close(fds->fd_prev[0]);
	close(fds->fd_prev[1]);
	return (EXIT_SUCCESS);
}

void	last_cmd(t_pipe_fds *fds, t_command *cmd, char **env_array,
		t_shell_context *shell_ctx)
{
	char	*full_path;

	if (is_builtin(cmd))
	{
		if_is_builtin(fds, cmd, shell_ctx, 3);
		// dup2(fds->fd[0], STDIN_FILENO);
		// close(fds->fd[0]);
		// close(fds->fd[1]);
		// execute_builtin(cmd, shell_ctx->env_copy, shell_ctx->process);
		// exit(shell_ctx->process->last_exit_status);
	}
	if (is_executable(cmd->args[0]))
	{
		full_path = cmd->args[0];
	}
	else
	{
		full_path = find_full_path(cmd->args[0], env_array);
		if (!full_path)
		{
			printf("minishell: %s: No such file or directory\n", cmd->args[0]);
			exit(127);
		}
	}
	if (dup2(fds->fd[0], STDIN_FILENO) == -1)
	{
		perror("dup2 failed");
		exit(EXIT_FAILURE);
	}
	close(fds->fd[1]);
	close(fds->fd[0]);
	// close(arg_list->file2);
	execve(full_path, cmd->args, env_array);
	printf("minishell: %s: %s\n", cmd->args[0], strerror(errno));
	exit(1);
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

int	main_pipes_process(t_ast *ast, t_shell_context *shell_ctx)
{
	t_pipe_fds	fds;
	int			i;
	char		**env_array;
	t_command	*cmds[1024];
	int			cmd_count;
	pid_t		*pid;
	int			status;

	i = 1;
	cmd_count = 0;
	collect_commands_in_order(ast, cmds, &cmd_count);
	pid = malloc(sizeof(pid_t) * cmd_count);
	if (!pid)
	{
		perror("malloc");
		return (EXIT_FAILURE);
	}
	// Debug
	for (int j = 0; j < cmd_count; j++)
	{
		for (int k = 0; cmds[j]->args[k] != NULL; k++)
		{
			printf("debug(pipes): cmd N %d, %s\n", j, cmds[j]->args[k]);
		}
	}
	// end
	env_array = create_env_array(shell_ctx->env_copy);
	// first cmd
	if (pipe(fds.fd) == -1)
	{
		ft_putstr_fd("Pipe failed\n", 2);
		set_exit_status(shell_ctx->process, 1);
		free(pid);
		free_splitted(env_array);
		return (EXIT_FAILURE);
	}
	if (create_fork(&pid[0], shell_ctx->process) == -1)
	{
		close(fds.fd[0]);
		close(fds.fd[1]);
		free(pid);
		free_splitted(env_array);
		return (EXIT_FAILURE);
	}
	if (pid[0] == 0)
	{
		first_cmd(&fds, cmds[0], env_array, shell_ctx);
	}
	while (i < cmd_count - 1)
	{
		if (pipe(fds.fd) == -1)
		{
			ft_putstr_fd("Pipe failed\n", 2);
			set_exit_status(shell_ctx->process, 1);
			free(pid);
			free_splitted(env_array);
			return (EXIT_FAILURE);
		}
		if (process_middle_cmd(pid[i], &fds, cmds[i], env_array, shell_ctx) == -1)
		{
			close(fds.fd[0]);
			close(fds.fd[1]);
			free(pid);
			free_splitted(env_array);
			return (EXIT_FAILURE);
		}
		i++;
	}
	if (create_fork(&pid[cmd_count - 1], shell_ctx->process) == -1)
	{
		close(fds.fd[0]);
		close(fds.fd[1]);
		free(pid);
		free_splitted(env_array);
		return (EXIT_FAILURE);
	}
	if (pid[cmd_count - 1] == 0)
	{
		last_cmd(&fds, cmds[cmd_count - 1], env_array, shell_ctx);
	}
	close(fds.fd[0]);
	close(fds.fd[1]);
	i = 0;
	while (i < cmd_count)
	{
		waitpid(pid[i], &status, 0);
		if (WIFEXITED(status))
			set_exit_status(shell_ctx->process, WEXITSTATUS(status));
		else if (WIFSIGNALED(status))
			set_exit_status(shell_ctx->process, 128 + WTERMSIG(status));
		else
			set_exit_status(shell_ctx->process, 1);
		i++;
	}
	free(pid);
	free_splitted(env_array);
	return (EXIT_SUCCESS);
}


// int	main_pipes_process(t_ast *ast, t_shell_context *shell_ctx)
// {
// 	t_pipe_fds	fds;
// 	int			i;
// 	char		**env_array;
// 	t_command	*cmds[1024];
// 	int			cmd_count;
// 	pid_t		*pid;
// 	int			status;

// 	cmd_count = 0;
// 	i = 0;
// 	cmd_count = 0;
// 	collect_commands_in_order(ast, cmds, &cmd_count);
// 	// debug
// 	for (int j = 0; j < cmd_count; j++)
// 	{
// 		for (int k = 0; cmds[j]->args[k] != NULL; k++)
// 		{
// 			printf("debug(pipes): cmd N %d, %s\n", j, cmds[j]->args[k]);
// 		}
// 	}
// 	// debug
// 	env_array = create_env_array(shell_ctx->env_copy);
// 	if (pipe(fds.fd) == -1)
// 	{
// 		ft_putstr_fd("Pipe failed\n", 2);
// 		set_exit_status(shell_ctx->process, 1);
// 		free_splitted(env_array);
// 		return (EXIT_FAILURE);
// 	}
// 	if (create_fork(&pid, shell_ctx->process) == EXIT_FAILURE)
// 	{
// 		close(fds.fd[0]);
// 		close(fds.fd[1]);
// 		free(pid);
// 		free_splitted(env_array);
// 		return (EXIT_FAILURE);
// 	}
// 	if (pid == 0)
// 	{
// 		printf("Debug: first command\n");
// 		first_cmd(&fds, cmds[0], env_array, shell_ctx);
// 	}
// 	while (i < cmd_count - 1)
// 	{
// 		printf("Debug: middle command\n");
// 		if (process_middle_cmd(pid, &fds, cmds[i], env_array,
// 				shell_ctx) == EXIT_FAILURE)
// 		{
// 			close(fds.fd[0]);
// 			close(fds.fd[1]);
// 			free_splitted(env_array);
// 			return (EXIT_FAILURE);
// 		}
// 		i++;
// 	}
// 	if (create_fork(&pid, shell_ctx->process) == EXIT_FAILURE)
// 	{
// 		close(fds.fd[0]);
// 		close(fds.fd[1]);
// 		free_splitted(env_array);
// 		return (EXIT_FAILURE);
// 	}
// 	if (pid == 0)
// 	{
// 		printf("Debug: last command\n");
// 		last_cmd(&fds, cmds[cmd_count - 1], env_array, shell_ctx);
// 	}
// 	close(fds.fd[0]);
// 	close(fds.fd[1]);
// 	i = 0;
// 	// while (wait(NULL) > 0)
// 	// 	;
// 	free_splitted(env_array);
// 	return (EXIT_SUCCESS);
// }
