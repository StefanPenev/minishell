/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   external_commands.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anilchen <anilchen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/18 13:32:28 by anilchen          #+#    #+#             */
/*   Updated: 2024/11/21 16:27:13 by anilchen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
			printf("minishell: %s: No such file or directory\n", cmd->args[0]);
			exit(127);
		}
		execve(full_path, cmd->args, env_array);
		free(full_path);
	}
	// If execve fails
	printf("minishell: %s: %s\n", cmd->args[0], strerror(errno));
	exit(1);
}

void handle_parent_process(pid_t main_pid, t_process *process)
{
    int status;

    waitpid(main_pid, &status, 0);
    if (WIFEXITED(status))
    {
        set_exit_status(process, WEXITSTATUS(status));
       // process->last_exit_status = WEXITSTATUS(status);
        printf("Debug: Child exited with status: %d\n", process->last_exit_status);
    }
    else if (WIFSIGNALED(status))
    {
        printf("Process was killed by signal %d\n", WTERMSIG(status));
        set_exit_status(process, 128 + WTERMSIG(status));
        //process->last_exit_status = 128 + WTERMSIG(status);
    }
    else
    {
        set_exit_status(process, 1);
        //process->last_exit_status = 1;
    }
}


int	execute_external_commands(t_command *cmd, t_env *env_copy,
		t_process *process)
{
	pid_t	main_pid;
	char	**env_array;

    //ask Stefan about checking for NULL arguments, delete if he have it
	if (cmd->args == NULL || cmd->args[0] == NULL)
	{
		printf("minishell: Command not found\n");
		set_exit_status(process, 127);
		return (EXIT_FAILURE);
	}
    //create env_array
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
		handle_parent_process(main_pid, process);
	free_splitted(env_array);
	return (EXIT_SUCCESS);
}
