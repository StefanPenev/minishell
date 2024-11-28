/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_cd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anilchen <anilchen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/14 13:11:23 by anilchen          #+#    #+#             */
/*   Updated: 2024/11/28 20:05:25 by anilchen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "process.h"

int	cd_error(char *str, char *old_path, t_process *process)
{
	write(2, &str, sizeof(str));
	if (old_path)
	{
		free(old_path);
	}
	set_exit_status(process, 1);
	return (EXIT_FAILURE);
}

int	chdir_error(const char *path, char *old_path, t_process *process)
{
	write(2, "minishell: cd: ", 15);
	write(2, path, ft_strlen(path));
	write(2, ": No such file or directory\n", 28);
	free(old_path);
	set_exit_status(process, 1);
	return (EXIT_FAILURE);
}

int	cd_to_home(t_env *env_copy, t_process *process)
{
	char	cwd[PATH_MAX];
	char	*new_path;
	char	*old_path;

	old_path = NULL;
	if (getcwd(cwd, sizeof(cwd)) == NULL)
		return (cd_error("minishell: getcwd", old_path, process));
	old_path = ft_strdup(cwd);
	new_path = get_env_value("HOME", env_copy, process);
	if (new_path == NULL || chdir(new_path) != 0)
		return (cd_error("minishell: cd: HOME not set or inaccessible\n",
				old_path, process));
	my_setenv("PWD", new_path, env_copy);
	my_setenv("OLDPWD", old_path, env_copy);
	free(old_path);
	set_exit_status(process, 0);
	return (EXIT_SUCCESS);
}

int	cd_to_path(const char *path, t_env *env_copy, t_process *process)
{
	char	cwd[PATH_MAX];
	char	*new_path;
	char	*old_path;

	old_path = NULL;
	if (getcwd(cwd, sizeof(cwd)) == NULL)
		return (cd_error("minishell: getcwd", old_path, process));
	old_path = ft_strdup(cwd);
	if (ft_strcmp(path, "..") == 0)
	{
		if (chdir("..") != 0)
			return (chdir_error(path, old_path, process));
	}
	else
	{
		if (chdir(path) != 0)
			return (chdir_error(path, old_path, process));
	}
	if (getcwd(cwd, sizeof(cwd)) == NULL)
		return (cd_error("minishell: getcwd", old_path, process));
	new_path = ft_strdup(cwd);
	my_setenv("PWD", new_path, env_copy);
	my_setenv("OLDPWD", old_path, env_copy);
	free(new_path);
	free(old_path);
	set_exit_status(process, 0);
	return (EXIT_SUCCESS);
}

// int	cd_to_points(t_env *env_copy, t_process *process)
// {
// 	char	cwd[PATH_MAX];
// 	char	*new_path;
// 	char	*old_path;

// 	old_path = NULL;
// 	if (getcwd(cwd, sizeof(cwd)) == NULL)
// 		return (cd_error("minishell: getcwd", old_path, process));
// 	old_path = ft_strdup(cwd);
// 	if (chdir("..") != 0)
// 		return (cd_error("minishell: cd: Failed to change to parent directory\n",
// 				old_path, process));
// 	if (getcwd(cwd, sizeof(cwd)) == NULL)
// 		return (cd_error("minishell: getcwd", old_path, process));
// 	new_path = ft_strdup(cwd);
// 	my_setenv("PWD", new_path, env_copy);
// 	my_setenv("OLDPWD", old_path, env_copy);
// 	free(new_path);
// 	free(old_path);
// 	set_exit_status(process, 0);
// 	return (EXIT_SUCCESS);
// }

// int	cd_to_path(const char *path, t_env *env_copy, t_process *process)
// {
// 	char	cwd[PATH_MAX];
// 	char	*new_path;
// 	char	*old_path;

// 	old_path = NULL;
// 	if (getcwd(cwd, sizeof(cwd)) == NULL)
// 		return (cd_error("minishell: getcwd", old_path, process));
// 	old_path = ft_strdup(cwd);
// 	if (chdir(path) != 0)
// 		return (chdir_error(path, old_path, process));
// 	if (getcwd(cwd, sizeof(cwd)) == NULL)
// 		return (cd_error("minishell: getcwd", old_path, process));
// 	new_path = ft_strdup(cwd);
// 	my_setenv("PWD", new_path, env_copy);
// 	my_setenv("OLDPWD", old_path, env_copy);
// 	free(new_path);
// 	free(old_path);
// 	set_exit_status(process, 0);
// 	return (EXIT_SUCCESS);
// }

int	execute_cd(t_command *cmd, t_env *env_copy, t_process *process)
{
	int	status;

	status = 0;
	if (cmd->args[1] == NULL || ft_strcmp(cmd->args[1], "~" == 0))
	{
		status = cd_to_home(env_copy, process);
		return (status);
	}
	if (cmd->args[2] != NULL)
	{
		write(2, "minishell: cd: too many arguments\n", 34);
		set_exit_status(process, 1);
		return (EXIT_FAILURE);
	}
	status = cd_to_path(cmd->args[1], env_copy, process);
	return (status);
}

// int	execute_cd(t_command *cmd, t_env *env_copy, t_process *process)
// {
// 	char	*new_path;
// 	char	*old_path;
// 	char	cwd[PATH_MAX];

// 	new_path = NULL;
// 	old_path = NULL;
// 	if (cmd->args[1] == NULL)
// 	{
// 		if (getcwd(cwd, sizeof(cwd)) == NULL)
// 		{
// 			perror("minishell: getcwd");
// 			set_exit_status(process, 1);
// 			return (EXIT_FAILURE);
// 		}
// 		old_path = ft_strdup(cwd);
// 		new_path = get_env_value("HOME", env_copy, process);
// 		chdir(new_path);
// 		my_setenv("PWD", new_path, env_copy);
// 		my_setenv("OLDPWD", old_path, env_copy);
// 		set_exit_status(process, 0);
// 		return (EXIT_SUCCESS);
// 	}
// 	if (cmd->args[2] != NULL)
// 	{
// 		write(2, "minishell: cd: too many arguments\n", 33);
// 		set_exit_status(process, 1);
// 		return (EXIT_FAILURE);
// 	}
// 	if (getcwd(cwd, sizeof(cwd)) == NULL)
// 	{
// 		perror("minishell: getcwd");
// 		set_exit_status(process, 1);
// 		return (EXIT_FAILURE);
// 	}
// 	old_path = ft_strdup(cwd);
// 	if (chdir(cmd->args[1]) != 0)
// 	{
// 		write(2, "minishell: cd: ", 15);
// 		write(2, cmd->args[1], ft_strlen(cmd->args[1]));
// 		write(2, ": No such file or directory\n", 28);
// 		free(old_path);
// 		set_exit_status(process, 1);
// 		return (EXIT_FAILURE);
// 	}
// 	//
// 	if (getcwd(cwd, sizeof(cwd)) == NULL)
// 	{
// 		perror("minishell: getcwd");
// 		free(old_path);
// 		set_exit_status(process, 1);
// 		return (EXIT_FAILURE);
// 	}
// 	new_path = ft_strdup(cwd);
// 	// set enviroment variables
// 	my_setenv("PWD", new_path, env_copy);
// 	my_setenv("OLDPWD", old_path, env_copy);
// 	set_exit_status(process, 0);
// 	return (EXIT_SUCCESS);
// }
