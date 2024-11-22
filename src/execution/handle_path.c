/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_path.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anilchen <anilchen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/21 15:49:13 by anilchen          #+#    #+#             */
/*   Updated: 2024/11/22 16:14:38 by anilchen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "process.h"

// find value in env array(not in linked list, its another function!)
char	*get_env(char *name, char **env_array)
{
	int	i;
	int	j;

	i = 0;
	while (env_array[i])
	{
		j = 0;
		while (env_array[i][j] && env_array[i][j] != '=')
			j++;
		if (ft_strncmp(env_array[i], name, j) == 0 && name[j] == '\0')
			return (env_array[i] + j + 1);
		i++;
	}
	return (NULL);
}

// find path to executable.
char	*find_full_path(char *args, char **env_array)
{
	char	*cmd_path;
	char	**all_paths;
	int		i;
	char	*env_path;

	i = 0;
	// getenv only returns line, contains all paths
	// and getenv function  from library uses envp, not custom array
	env_path = get_env("PATH", env_array);
	// so we split it by ':'
	all_paths = ft_split(env_path, ':');
	// and searching for path where our command exists
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
	return (0);
}
