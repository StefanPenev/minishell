/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_cd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anilchen <anilchen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/14 13:11:23 by anilchen          #+#    #+#             */
/*   Updated: 2024/11/29 17:24:08 by anilchen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "process.h"

#define ERR_NO_DIR "No such file or directory\n"
#define ERR_CWD "minishell: getcwd failed\n"
#define ERR_CD_HOME "minishell: cd: HOME not set or inaccessible\n"
#define ERR_TOO_MANY_ARGS "minishell: cd: too many arguments\n"

// Handles errors during the execution of the `cd` command.
// Logs the error message and frees resources such as `old_path`.
// Sets the exit status for the process to indicate failure.
// Parameters:
//   - error_type: String describing the type of error.
//   - path: The target path that caused the error (can be NULL).
//   - old_path: Pointer to the old working directory to be freed
//(if allocated).
//   - process: Pointer to the process structure to update the exit status.
// Returns:
//   - EXIT_FAILURE: Always returns failure after handling the error.

int	handle_cd_error(const char *error_type, const char *path, char *old_path,
		t_process *process)
{
	if (error_type)
		write(2, error_type, ft_strlen(error_type));
	if (path)
	{
		write(2, path, ft_strlen(path));
		write(2, ": ", 2);
	}
	write(2, ERR_NO_DIR, ft_strlen(ERR_NO_DIR));
	if (old_path)
		free(old_path);
	set_exit_status(process, 1);
	return (EXIT_FAILURE);
}

// Updates the environment variables `PWD` and `OLDPWD` after a directory
// change. Frees the memory used by the old and new paths.
// Sets the process's exit status to success.
// Parameters:
//   - new_path: Pointer to the new working directory path.
//   - old_path: Pointer to the old working directory path.
//   - env_copy: Pointer to the environment variable list to be updated.
//   - process: Pointer to the process structure to update the exit status.
// Returns:
//   - EXIT_SUCCESS: After successfully updating the environment variables.

int	update_env_paths(char *new_path, char *old_path, t_env *env_copy,
		t_process *process)
{
	my_setenv("PWD", new_path, env_copy);
	my_setenv("OLDPWD", old_path, env_copy);
	free(new_path);
	free(old_path);
	set_exit_status(process, 0);
	return (EXIT_SUCCESS);
}

// Changes the working directory to the user's home directory.
// Updates the environment variables `PWD` and `OLDPWD`.
// Handles errors if the `HOME` environment variable is not set or if access
// fails.
// Parameters:
//   - env_copy: Pointer to the environment variable list.
//   - process: Pointer to the process structure to update the exit status.
// Returns:
//   - EXIT_SUCCESS: If the directory change is successful.
//   - EXIT_FAILURE: If any error occurs during the operation.

int	cd_to_home(t_env *env_copy, t_process *process)
{
	char	cwd[PATH_MAX];
	char	*new_path;
	char	*old_path;

	old_path = NULL;
	if (getcwd(cwd, sizeof(cwd)) == NULL)
		return (handle_cd_error(ERR_CWD, new_path, old_path, process));
	old_path = ft_strdup(cwd);
	new_path = get_env_value("HOME", env_copy, process);
	if (new_path == NULL || chdir(new_path) != 0)
		return (handle_cd_error(ERR_CD_HOME, new_path, old_path, process));
	update_env_paths(new_path, old_path, env_copy, process);
	return (EXIT_SUCCESS);
}

// Changes the working directory to the specified path.
// Updates the environment variables `PWD` and `OLDPWD`.
// Handles errors if the path does not exist or access fails.
// Parameters:
//   - path: Target path to change to.
//   - env_copy: Pointer to the environment variable list.
//   - process: Pointer to the process structure to update the exit status.
// Returns:
//   - EXIT_SUCCESS: If the directory change is successful.
//   - EXIT_FAILURE: If any error occurs during the operation.

int	cd_to_path(const char *path, t_env *env_copy, t_process *process)
{
	char	cwd[PATH_MAX];
	char	*new_path;
	char	*old_path;

	old_path = NULL;
	if (getcwd(cwd, sizeof(cwd)) == NULL)
		return (handle_cd_error(ERR_CWD, new_path, old_path, process));
	old_path = ft_strdup(cwd);
	if (ft_strcmp(path, "..") == 0)
	{
		if (chdir("..") != 0)
			return (handle_cd_error(ERR_NO_DIR, path, old_path, process));
	}
	else
	{
		if (chdir(path) != 0)
			return (handle_cd_error(ERR_NO_DIR, path, old_path, process));
	}
	if (getcwd(cwd, sizeof(cwd)) == NULL)
		return (handle_cd_error(ERR_CWD, path, old_path, process));
	new_path = ft_strdup(cwd);
	update_env_paths(new_path, old_path, env_copy, process);
	return (EXIT_SUCCESS);
}

// Executes the `cd` command based on the provided arguments.
// Handles cases like no arguments (`cd` to home) and invalid arguments.
// Parameters:
//   - cmd: Pointer to the command structure containing arguments.
//   - env_copy: Pointer to the environment variable list.
//   - process: Pointer to the process structure to update the exit status.
// Returns:
//   - EXIT_SUCCESS: If the directory change is successful.
//   - EXIT_FAILURE: If an error occurs.

int	execute_cd(t_command *cmd, t_env *env_copy, t_process *process)
{
	int	status;

	status = 0;
	if (cmd->args[1] == NULL || ft_strcmp(cmd->args[1], "~") == 0)
	{
		status = cd_to_home(env_copy, process);
		return (status);
	}
	if (cmd->args[2] != NULL)
	{
		write(2, ERR_TOO_MANY_ARGS, 34);
		set_exit_status(process, 1);
		return (EXIT_FAILURE);
	}
	status = cd_to_path(cmd->args[1], env_copy, process);
	return (status);
}
