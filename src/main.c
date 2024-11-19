/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anilchen <anilchen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/13 11:20:29 by anilchen          #+#    #+#             */
/*   Updated: 2024/11/19 12:57:21 by anilchen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "process.h"

// cc exec_commands.c -L./libft -lft -lreadline -o minishell

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// t_ctrl	*init_ctrl(void)
// {
// 	t_ctrl	*ctrl;

// 	ctrl = malloc(sizeof(t_ctrl));
// 	if (!ctrl)
// 		return (NULL);
// 	ctrl->env = NULL;
// 	ctrl->process.last_exit_status = 0;
// 	return (ctrl);
// }

int	main(int argc, char *argv[], char **envp)
{
	char		*line;
	char		**splitted;
	t_env		*env_copy;
	t_process	process;

	// t_ctrl	*ctrl;
	// ctrl = init_ctrl();
	// if (!ctrl)
	// 	return (1);
	process.last_exit_status = 0;
	env_copy = init_env(envp);
	while (1)
	{
		line = readline("minishell> ");
		if (!line)
			break ;
		splitted = ft_split(line, ' ');
		// for (int j = 0; splitted[j] != NULL; j++)
		// {
		// 	printf("Debug: Split part[%d]: %s\n", j, splitted[j]);
		// }
		if (splitted[0] == NULL)
		{
			free(line);
			cleanup(splitted);
			continue ;
		}
		if (ft_strncmp(splitted[0], "./", 2) == 0)
		{
			execute_external_commands(splitted, env_copy, &process);
		}
		else if (ft_strcmp(splitted[0], "echo") == 0)
		{
			minishell_echo(splitted, env_copy, &process);
		}
		else if (ft_strcmp(splitted[0], "cd") == 0)
		{
			minishell_cd(splitted, env_copy, &process);
		}
		else if (ft_strcmp(splitted[0], "pwd") == 0)
		{
			minishell_pwd(splitted, env_copy, &process);
		}
		else if (ft_strcmp(splitted[0], "export") == 0)
		{
			minishell_export(splitted, env_copy, &process);
		}
		else if (ft_strcmp(splitted[0], "env") == 0)
		{
			minishell_env(splitted, env_copy, &process);
		}
		else if (ft_strcmp(splitted[0], "unset") == 0)
		{
			minishell_unset(splitted, env_copy, &process);
		}
		else if (ft_strcmp(splitted[0], "exit") == 0)
		{
			minishell_exit(splitted, env_copy, &process);
		}
		else
		{
			printf("Command not found: %s\n", splitted[0]);
			set_exit_status(&process, 127);
			// cleanup(splitted);
			// free_env((t_env **)&env_copy);
			// return (EXIT_FAILURE);
		}
		cleanup(splitted);
		free(line);
	}
	free_env((t_env **)&env_copy);
	// free_ctrl(ctrl);
	// free_env_copy(&env_copy);
	return (EXIT_SUCCESS);
}

// int	main(int argc, char *argv[], char **envp)
// {
// 	char	*line;
// 	char	**splitted;
// 	t_env	*env_copy;
// 	t_ctrl	*ctrl;

// 	ctrl = init_shell();
// 	if (!ctrl)
// 		return (1);
// 	env_copy = init_env(envp);
// 	while (1)
// 	{
// 		line = readline("minishell> ");
// 		if (!line)
// 			break ;
// 		splitted = ft_split(line, ' ');
// 		if (splitted[0] == NULL)
// 		{
// 			free(line);
// 			cleanup(splitted);
// 			continue ;
// 		}
// 		if (ft_strcmp(splitted[0], "echo") == 0)
// 		{
// 			minishell_echo(splitted, env_copy);
// 		}
// 		else if (ft_strcmp(splitted[0], "cd") == 0)
// 		{
// 			minishell_cd(splitted, env_copy);
// 		}
// 		else if (ft_strcmp(splitted[0], "pwd") == 0)
// 		{
// 			minishell_pwd(splitted, env_copy);
// 		}
// 		else if (ft_strcmp(splitted[0], "export") == 0)
// 		{
// 			minishell_export(splitted, env_copy);
// 		}
// 		else if (ft_strcmp(splitted[0], "env") == 0)
// 		{
// 			minishell_env(env_copy);
// 		}
// 		else if (ft_strcmp(splitted[0], "unset") == 0)
// 		{
// 			minishell_unset(splitted, env_copy);
// 		}
// 		else if (ft_strcmp(splitted[0], "exit") == 0)
// 		{
// 			minishell_exit(splitted, env_copy);
// 		}
// 		else
// 		{
// 			printf("Command not found: %s\n", splitted[0]);
// 			return (EXIT_FAILURE);
// 		}
// 		cleanup(splitted);
// 		free(line);
// 	}
// 	free_ctrl(ctrl);
// 	// minishell_lstclear((t_env **)&env_copy);
// 	// free_env_copy(&env_copy);
// 	return (EXIT_SUCCESS);
// }

// int	main(int argc, char *argv[], char **envp)
// {
// 	char		*line;
// 	char		**splitted;
// 	t_process	**env_copy;

// 	env_copy = envp;
// 	line = readline("minishell> ");
// 	splitted = ft_split(line, ' ');
// 	if (ft_strcmp(splitted[0], "echo") == 0)
// 	{
// 		minishell_echo(splitted);
// 	}
// 	else if (ft_strcmp(splitted[0], "cd") == 0)
// 	{
// 		minishell_cd(splitted);
// 	}
// 	else
// 	{
// 		printf("arguments do not provided or wrong\n");
// 		cleanup(splitted);
// 		free(line);
// 		return (1);
// 	}
// 	cleanup(splitted);
// 	free(line);
// 	return (0);
// }
