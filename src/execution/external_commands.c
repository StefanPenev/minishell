/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   external_commands.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anilchen <anilchen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/18 13:32:28 by anilchen          #+#    #+#             */
/*   Updated: 2024/11/28 19:12:23 by anilchen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "process.h"

void	handle_child_process(t_command *cmd, char **env_array)
{
	char	*full_path;

	// if path provided, like /bin/ls -l
	if (is_executable(cmd->args[0]))
		execve(cmd->args[0], cmd->args, env_array);
	// if not we have to search for path
	else
	{
		full_path = find_full_path(cmd->args[0], env_array);
		if (!full_path)
		{
			printf("minishell: Command '%s' not found\n", cmd->args[0]);
			// printf("minishell: %s: No such file or directory\n",
			//	cmd->args[0]);
			exit(127);
		}
		execve(full_path, cmd->args, env_array);
		free(full_path);
	}
	// If execve fails
	printf("minishell: %s: %s\n", cmd->args[0], strerror(errno));
	exit(1);
}

void	handle_child_exit_status(pid_t main_pid, t_process *process)
{
	int	status;

	if (main_pid <= 0)
	{
		fprintf(stderr, "[ERROR] Invalid PID: %d\n", main_pid);
		return ;
	}

	status = 0;
	printf("Debug(handle_child_exit_status): Waiting for PID: %d\n", main_pid);

	if (waitpid(main_pid, &status, 0) == -1)
	{
		perror("[ERROR] waitpid failed");
		set_exit_status(process, 1);
		return ;
	}

	if (WIFEXITED(status))
	{
		int exit_code = WEXITSTATUS(status);
		printf("[DEBUG] Process %d exited with status %d\n", main_pid, exit_code);
		set_exit_status(process, exit_code);
	}
	else if (WIFSIGNALED(status))
	{
		int signal = WTERMSIG(status);
		printf("[DEBUG] Process %d was killed by signal %d (%s)\n", 
		       main_pid, signal, strsignal(signal));
		set_exit_status(process, 128 + signal);
	}
	else if (WIFSTOPPED(status))
	{
		int signal = WSTOPSIG(status);
		printf("[DEBUG] Process %d was stopped by signal %d (%s)\n",
		       main_pid, signal, strsignal(signal));
		// В зависимости от логики, можно обработать остановленный процесс,
		// но обычно это не требуется для shell.
		set_exit_status(process, 1);
	}
	else
	{
		printf("[DEBUG] Process %d ended in an unknown way\n", main_pid);
		set_exit_status(process, 1);
	}
}

int	execute_external_commands(t_command *cmd, t_env *env_copy,
		t_process *process)
{
	pid_t	main_pid;
	char	**env_array;

	// ask Stefan about checking for NULL arguments, delete if he have it
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
