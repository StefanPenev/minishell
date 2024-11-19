/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   external_commands.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anilchen <anilchen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/18 13:32:28 by anilchen          #+#    #+#             */
/*   Updated: 2024/11/18 15:38:03 by anilchen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "process.h"

// if file exists and executable
int	is_executable(const char *path)
{
	if (access(path, X_OK) == 0)
	{
		return (1);
	}
	else
	{
		return (0);
	}
}

char	*get_env(char *name, char **env_array)
{
	int	i;
	int	j;

	i = 0;
	while (env_array[i])
	{
		j = 0;
		while (env_array[i][j] && env_array[i][j] != '=')
		{
			j++;
		}
		if (ft_strncmp(env_array[i], name, j) == 0 && name[j] == '\0')
		{
			return (env_array[i] + j + 1);
		}
		i++;
	}
	return (NULL);
}

char	*find_full_path(char *args, char **env_array)
{
	char	*cmd_path;
	char	**all_paths;
	int		i;
	char	*env_path;

	i = 0;
	// getenv only returns line, contains all paths
	env_path = get_env("PATH", env_array);
	// so we split it by ':'
	all_paths = ft_split(env_path, ':');
	// and searching for path where our command exists
	while (all_paths[i] != NULL)
	{
		cmd_path = ft_strjoin_three(all_paths[i], "/", args);
		if (access(cmd_path, X_OK) == 0)
		{
			cleanup(all_paths);
			return (cmd_path);
		}
		free(cmd_path);
		i++;
	}
	cleanup(all_paths);
	return (0);
}

char	**create_env_array(t_env *env_copy)
{
	t_env	*cur;
	int		i;
	int		env_count;
	char	**env_array;
	char	*key_value;

	// env_count = 0;
	// count elements in env_list
	env_count = minishell_lstsize(env_copy);
	// +1 for NULL
	env_array = malloc(sizeof(char *) * (env_count + 1));
	// kopying from list to array
	cur = env_copy;
	i = 0;
	while (cur != NULL)
	{
		key_value = ft_strjoin_three(cur->key, "=", cur->value);
		if (!key_value)
		{
			cleanup(env_array);
			return (NULL);
		}
		env_array[i] = key_value;
		cur = cur->next;
		i++;
	}
	env_array[i] = NULL;
	return (env_array);
}

int	execute_external_commands(char **splitted, t_env *env_copy,
		t_process *process)
{
	pid_t	main_pid;
	char	*full_path;
	char	**env_array;
	int		status;

	// just to be sure, can be deleted because I have same check in the main
	if (splitted == NULL || splitted[0] == NULL)
	{
		printf("minishell: Command not found\n");
		set_exit_status(process, 127);
		return (EXIT_FAILURE);
	}
	printf("Debug: Command to execute: %s\n", splitted[0]); // debug
	// in theory another path can be added. So its better to use my own enviroment
	// instead of envp from main
	env_array = create_env_array(env_copy);
	main_pid = fork();
	if (main_pid == -1)
	{
		printf("Fork failed\n");
		set_exit_status(process, 1);
		return (EXIT_FAILURE);
	}
	if (main_pid == 0)
	{
		// child
		if (is_executable(splitted[0]))
		{
			execve(splitted[0], splitted, env_array);
		}
		// trying to find path if it wasnt before
		else
		{
			full_path = find_full_path(splitted[0], env_array);
			if (!full_path)
			{
				printf("minishell: %s: command not found\n", splitted[0]);
				set_exit_status(process, 127);
				exit(127);
			}
			execve(full_path, splitted, env_array);
			free(full_path);
		}
		// if execve returns here, it means error
		printf("minishell: %s: %s\n", splitted[0], strerror(errno));
		exit(1);
	}
	else
	{
		// parent process waits exactly this pid
		waitpid(main_pid, &status, 0);
		if (WIFEXITED(status))
		{
			process->last_exit_status = WEXITSTATUS(status);
		}
		else if (WIFSIGNALED(status))
		{
			// if process was killed by signal
			printf("Process was killed by signal %d\n", WTERMSIG(status));
			process->last_exit_status = 128 + WTERMSIG(status);
		}
		else
		{
			// another cases
			process->last_exit_status = 1;
		}
		cleanup(env_array);
		return (EXIT_SUCCESS);
	}
}
