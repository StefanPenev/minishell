/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_path.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anilchen <anilchen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/21 15:49:13 by anilchen          #+#    #+#             */
/*   Updated: 2024/11/29 13:56:19 by anilchen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "process.h"

// Searches for a variable's value in the environment array (not linked list).
// Iterates through the array, splits each string at the '=' character,
// and compares the variable name to the given name.

char	*get_env(char *name, char **env_array)
{
	int	i;
	int	j;

	i = 0;
	if (!name || !env_array)
		return (NULL);
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

// Finds the full path to an executable command using
// the "PATH" environment variable.
// Splits the "PATH" variable into directories, appends the command,
// and checks if the executable exists in each directory.
// getenv function  from library uses envp, not custom array

char	*find_full_path(char *args, char **env_array)
{
	char	*cmd_path;
	char	**all_paths;
	int		i;
	char	*env_path;

	i = 0;
	if (!args || !env_array)
		return (NULL);
	env_path = get_env("PATH", env_array);
	if (!env_path)
		return (NULL);
	all_paths = ft_split(env_path, ':');
	while (all_paths[i] != NULL)
	{
		cmd_path = ft_strjoin_three(all_paths[i], "/", args);
		if (is_executable(cmd_path))
		{
			free_splitted(all_paths);
			return (cmd_path);
		}
		free(cmd_path);
		i++;
	}
	free_splitted(all_paths);
	return (NULL);
}
