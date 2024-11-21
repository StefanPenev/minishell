/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anilchen <anilchen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/21 15:08:15 by anilchen          #+#    #+#             */
/*   Updated: 2024/11/21 16:52:26 by anilchen         ###   ########.fr       */
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
		return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}

void	first_cmd(t_pipe_fds *fds, t_command *cmd, char **env_array,
		t_env *env_copy, t_process *process)
{
	char	*full_path;

	if (is_builtin(cmd))
	{
		execute_builtin(cmd, env_copy, process);
		exit(process->last_exit_status);
	}
	// if (arg_list->heredoc_calling)
	// {
	// 	close(arg_list->file1);
	// 	arg_list->file1 = open(arg_list->file1_name, O_RDONLY);
	// 	if (arg_list->file1 == -1)
	// 	{
	// 		error_msg("Error reopening file1", arg_list, 1);
	// 	}
	// }
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
	if (dup2(fds->fd[1], STDOUT_FILENO) == -1)
	{
		perror("dup2 failed");
		exit(EXIT_FAILURE);
	}
	close(fds->fd[0]);
	close(fds->fd[1]);
	// close(arg_list->file1);
	execve(full_path, cmd->args, env_array);
	// If execve fails
	printf("minishell: %s: %s\n", cmd->args[0], strerror(errno));
	exit(EXIT_FAILURE);
}

void	execute_middle_cmd(t_pipe_fds *fds, t_command *cmd, char **env_array,
		t_env *env_copy, t_process *process)
{
	char	*full_path;
	if (is_builtin(cmd))
	{
		execute_builtin(cmd, env_copy, process);
		exit(process->last_exit_status);
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

int	process_middle_cmd(t_pipe_fds *fds, t_command *cmd, char **env_array,
		t_env *env_copy, t_process *process)
{
	pid_t	pid;

	fds->fd_prev[0] = fds->fd[0];
	fds->fd_prev[1] = fds->fd[1];
	if (pipe(fds->fd) == -1)
	{
		ft_putstr_fd("Pipe failed\n", 2);
		set_exit_status(process, 1);
		free_splitted(env_array);
		return (EXIT_FAILURE);
	}
	if (create_fork(&pid, process) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	if (pid == 0)
	{
		execute_middle_cmd(fds, cmd, env_array, env_copy, process);
	}
	close(fds->fd_prev[0]);
	close(fds->fd_prev[1]);
	return (EXIT_SUCCESS);
}

void	last_cmd(t_pipe_fds *fds, t_command *cmd, char **env_array,
		t_env *env_copy, t_process *process)
{
	char	*full_path;

	if (is_builtin(cmd))
	{
		execute_builtin(cmd, env_copy, process);
		exit(process->last_exit_status);
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

int	main_pipes_process(t_command *cmd, t_env *env_copy, t_process *process)
{
	pid_t pid;
	t_pipe_fds fds;
	int i = 1;
	char **env_array;

	env_array = create_env_array(env_copy);
	if (pipe(fds.fd) == -1)
	{
		ft_putstr_fd("Pipe failed\n", 2);
		set_exit_status(process, 1);
		free_splitted(env_array);
		return (EXIT_FAILURE);
	}
	if (create_fork(&pid, process) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	if (pid == 0)
	{
		first_cmd(&fds, cmd, env_array, env_copy, process);
	}
	while (cmd->args[i] != NULL)
	{
		process_middle_cmd(&fds, cmd, env_array, env_copy, process);
		i++;
	}
	if (create_fork(&pid, process) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	if (pid == 0)
		last_cmd(&fds, cmd, env_array, env_copy, process);
	close(fds.fd[0]);
	close(fds.fd[1]);
	while (wait(NULL) > 0)
		;
	free_splitted(env_array);
	return (EXIT_SUCCESS);
}