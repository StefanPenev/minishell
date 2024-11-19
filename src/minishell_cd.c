/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_cd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anilchen <anilchen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/14 13:11:23 by anilchen          #+#    #+#             */
/*   Updated: 2024/11/19 13:28:29 by anilchen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "process.h"

int	minishell_cd(char **splitted, t_env *env_copy, t_process *process)
{
	char	*new_path;
	char	*old_path;
	char	cwd[PATH_MAX];

	new_path = NULL;
	old_path = NULL;
	// should be path
	if (splitted[1] == NULL)
	{
		write(2, "minishell: cd: expect path to directory\n", 40);
		set_exit_status(process, 1);
		return (EXIT_FAILURE);
	}
	// shouldnt be more arguments than 1
	if (splitted[2] != NULL)
	{
		write(2, "minishell: cd: too many arguments\n", 33);
		set_exit_status(process, 1);
		return (EXIT_FAILURE);
	}
	// save old_path
	if (getcwd(cwd, sizeof(cwd)) == NULL)
	{
		perror("minishell: getcwd");
		set_exit_status(process, 1);
		return (EXIT_FAILURE);
	}
	old_path = ft_strdup(cwd);
	// handle new_path
	if (splitted[1][0] == '$')
	{
		// if path starts from '$'
		char *env_key = ft_strdup(splitted[1] + 1); // skip '$'
		new_path = get_env_value(env_key, env_copy, process);
		free(env_key);
		if (new_path == NULL || chdir(new_path) != 0)
		{
			write(2, "minishell: cd: ", 15);
			write(2, splitted[1], ft_strlen(splitted[1]));
			write(2, ": No such file or directory\n", 28);
			set_exit_status(process, 1);
			free(old_path);
			return (EXIT_FAILURE);
		}
	}
	else
	{
		// usual path
		// (if directory do not exist)
		if (chdir(splitted[1]) != 0)
		{
			write(2, "minishell: cd: ", 15);
			write(2, splitted[1], ft_strlen(splitted[1]));
			write(2, ": No such file or directory\n", 28);
			free(old_path);
			set_exit_status(process, 1);
			return (EXIT_FAILURE);
		}
		//
		if (getcwd(cwd, sizeof(cwd)) == NULL)
		{
			perror("minishell: getcwd");
			free(old_path);
			set_exit_status(process, 1);
			return (EXIT_FAILURE);
		}
		new_path = ft_strdup(cwd);
	}
	// set enviroment variables
	my_setenv("PWD", new_path, env_copy);
	my_setenv("OLDPWD", old_path, env_copy);
	set_exit_status(process, 0);
	return (EXIT_SUCCESS);
}

// int	minishell_cd(char **splitted, t_env *env_copy, t_process *process)
// {
// 	char	*new_path;
// 	char	*old_path;
// 	char	*name;

// 	char cwd[PATH_MAX]; // PATH_MAX = 4096, just looks better =)
// 	// check if path provided
// 	if (splitted[1] == NULL)
// 	{
// 		write(2, "minishell: cd: ", 15);
// 		write(2, splitted[1], ft_strlen(splitted[1]));
// 		write(2, "expect path to directory\n", 25);
// 		set_exit_status(process, 1);
// 		return (EXIT_FAILURE);
// 	}
// 	// cd should be without any flags
// 	if (splitted[2] != NULL)
// 	{
// 		write(2, "minishell: cd: ", 15);
// 		write(2, "too many arguments\n", 19);
// 		set_exit_status(process, 1);
// 		return (EXIT_FAILURE);
// 	}
// 	// save old path
// 	if (getcwd(cwd, sizeof(cwd)) == NULL)
// 	{
// 		printf("minishell: getcwd: %s\n", strerror(errno));
// 		set_exit_status(process, 1);
// 		return (EXIT_FAILURE);
// 	}
// 	old_path = ft_strdup(cwd);
// 	// printf("Debug: old path = %s\n", old_path); // debug
// 	if (chdir(splitted[1]) != 0)
// 	{
// 		write(2, "minishell: cd: ", 15);
// 		write(2, "No such file or directory\n", 26);
// 		// printf("minishell: cd: %s: %s\n", splitted[1], strerror(errno));
// 		set_exit_status(process, 1);
// 		return (EXIT_FAILURE);
// 	}
// 	if (splitted[1][0] == '$')
// 	{
// 		name = ft_strdup(splitted[1] + 1);
// 		new_path = get_env_value(name, env_copy, process);
// 		free(name);
// 	}
// 	else if (splitted[1][0] != '.') // if absolyte way(starts not from ".."),
// 	// need to test with single dot
// 	{
// 		new_path = ft_strdup(splitted[1]); // just copy it to save in pwd
// 	}
// 	else
// 	// if symbolic
// 	{
// 		// save new path
// 		// need more tests for this part
// 		if (getcwd(cwd, sizeof(cwd)) != NULL) // get_current_dir_name
// 												//- get current working directory
// 		{
// 			new_path = ft_strdup(cwd);
// 			printf("Debug: new path = %s\n", new_path); // debug
// 		}
// 		else
// 		{
// 			printf("minishell: getcwd: %s\n", strerror(errno));
// 			set_exit_status(process, 1);
// 			return (EXIT_FAILURE);
// 		}
// 	}
// 	// set new pwd
// 	my_setenv("PWD", new_path, env_copy);
// 	// set old pwd
// 	my_setenv("OLDPWD", old_path, env_copy);
// 	// free(new_path);
// 	set_exit_status(process, 0);
// 	return (EXIT_SUCCESS);
// }

// int	minishell_cd(char **splitted, t_env *env_copy, t_process *process)
// {
// 	char	cwd[PATH_MAX];

// 	if (splitted[1] == NULL)
// 	{
// 		printf("minishell: cd: expect path to directory\n");
// 		set_exit_status(process, 1);
// 		return (EXIT_FAILURE);
// 	}
// 	if (splitted[2] != NULL)
// 	{
// 		printf("minishell: cd: too many arguments\n");
// 		set_exit_status(process, 1);
// 		return (EXIT_FAILURE);
// 	}
// 	if (chdir(splitted[1]) != 0)
// 	{
// 		printf("minishell: cd: %s: %s\n", splitted[1], strerror(errno));
// 		set_exit_status(process, 1);
// 		return (EXIT_FAILURE);
// 	}
// 	if (getcwd(cwd, sizeof(cwd)) != NULL)
// 	{
// 		my_setenv("PWD", ft_strdup(cwd), env_copy);
// 	}
// 	else
// 	{
// 		printf("minishell: getcwd");
// 		set_exit_status(process, 1);
// 		return (EXIT_FAILURE);
// 	}
// 	set_exit_status(process, 0);
// 	return (EXIT_SUCCESS);
// }
