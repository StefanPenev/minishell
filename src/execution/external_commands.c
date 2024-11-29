/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   external_commands.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anilchen <anilchen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/18 13:32:28 by anilchen          #+#    #+#             */
/*   Updated: 2024/11/29 17:24:49 by anilchen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "process.h"

// Waits for a child process to finish and processes its exit status.
// Updates the process exit status based on the child's termination type.
// Parameters:
//   - main_pid: PID of the child process to wait for.
//   - process: Pointer to the process structure to update the exit status.

void	handle_child_exit_status(pid_t main_pid, t_process *process)
{
	int	status;

	if (main_pid <= 0)
	{
		printf("[ERROR] Invalid PID: %d\n", main_pid);
		set_exit_status(process, 1);
		return ;
	}
	if (waitpid(main_pid, &status, 0) == -1)
	{
		perror("[ERROR] waitpid failed");
		set_exit_status(process, 1);
		return ;
	}
	if (WIFEXITED(status))
		set_exit_status(process, WEXITSTATUS(status));
	else if (WIFSIGNALED(status))
		set_exit_status(process, 128 + WTERMSIG(status));
	else
		set_exit_status(process, 1);
}

// Handles the execution of a child process in a shell-like environment.
// Executes a command by either finding it in the PATH or directly running it.
// Handles errors such as missing commands or invalid paths.
// Parameters:
//   - cmd: Pointer to the command structure containing arguments.
//   - env_array: Array of environment variables for the process.

void	handle_child_process(t_command *cmd, char **env_array)
{
	char		*full_path;
	struct stat	path_stat;

	if (stat(cmd->args[0], &path_stat) == 0 && S_ISDIR(path_stat.st_mode))
	{
		printf("minishell: %s: Is a directory\n", cmd->args[0]);
		exit(126);
	}
	if (is_executable(cmd->args[0]))
		execve(cmd->args[0], cmd->args, env_array);
	else
	{
		full_path = find_full_path(cmd->args[0], env_array);
		if (!full_path)
		{
			printf("minishell: Command '%s' not found\n", cmd->args[0]);
			exit(127);
		}
		execve(full_path, cmd->args, env_array);
		free(full_path);
	}
	printf("minishell: %s: %s\n", cmd->args[0], strerror(errno));
	exit(1);
}

// Executes an external command by forking a new process and waiting for it.
// Handles both direct executable paths and commands resolved via PATH.
// Parameters:
//   - cmd: Pointer to the command structure containing arguments.
//   - env_copy: Pointer to the environment variable list.
//   - process: Pointer to the process structure to manage the exit status.
// Returns:
//   - EXIT_SUCCESS: If the command is executed successfully.
//   - EXIT_FAILURE: If an error occurs.

int	execute_external_commands(t_command *cmd, t_env *env_copy,
		t_process *process)
{
	pid_t	main_pid;
	char	**env_array;

	if (cmd->args == NULL || cmd->args[0] == NULL)
	{
		printf("minishell: Command not found\n");
		set_exit_status(process, 127);
		return (EXIT_FAILURE);
	}
	env_array = create_env_array(env_copy);
	main_pid = fork();
	if (main_pid == -1)
	{
		ft_putstr_fd("Fork failed\n", 2);
		set_exit_status(process, 1);
		free_splitted(env_array);
		return (EXIT_FAILURE);
	}
	if (main_pid == 0)
		handle_child_process(cmd, env_array);
	else
		handle_child_exit_status(main_pid, process);
	free_splitted(env_array);
	return (EXIT_SUCCESS);
}
