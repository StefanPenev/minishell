/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_wildcard.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stfn <stfn@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/17 18:55:25 by stfn              #+#    #+#             */
/*   Updated: 2024/11/18 10:32:47 by stfn             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"
#include "minishell.h"

static int	add_file(char ***files, int *i, int *capacity,
	const char *file_name)
{
	if (*i >= *capacity)
	{
		if (!resize_array(files, capacity, *i))
			return (0);
	}
	(*files)[*i] = ft_strdup(file_name);
	if (!(*files)[*i])
	{
		perror("strdup failed");
		return (0);
	}
	(*i)++;
	return (1);
}

static int	read_entries(DIR *dp, char ***files, int *i, int *capacity)
{
	struct dirent	*entry;

	entry = readdir(dp);
	while (entry)
	{
		if (entry->d_name[0] != '.')
		{
			if (!add_file(files, i, capacity, entry->d_name))
				return (0);
		}
		entry = readdir(dp);
	}
	return (1);
}

static char	**allocate_files_array(int *capacity)
{
	char	**files;

	files = NULL;
	*capacity = INITIAL_CAPACITY;
	files = malloc(sizeof(char *) * (*capacity));
	if (!files)
		perror("malloc failed");
	return (files);
}

static char	**process_directory(DIR *dp)
{
	char	**files;
	int		capacity;
	int		i;

	files = NULL;
	capacity = 0;
	i = 0;
	files = allocate_files_array(&capacity);
	if (!files)
		return (NULL);
	if (!read_entries(dp, &files, &i, &capacity))
	{
		free_files(files, i);
		return (NULL);
	}
	files[i] = NULL;
	return (files);
}

char	**expand_wildcard(void)
{
	DIR		*dp;
	char	**files;

	dp = NULL;
	files = NULL;
	dp = opendir(".");
	if (!dp)
	{
		perror("opendir failed");
		return (NULL);
	}
	files = process_directory(dp);
	closedir(dp);
	return (files);
}

// char	**expand_wildcard(void)
// {
// 	int				i;
// 	int				j;
// 	int				capacity;
// 	char			**files;
// 	struct dirent	*entry;
// 	DIR				*dp;
// 	char			**new_files;

// 	dp = opendir(".");
// 	if (!dp)
// 	{
// 		perror("opendir failed");
// 		return (NULL);
// 	}
// 	capacity = INITIAL_CAPACITY;
// 	files = malloc(sizeof(char *) * capacity);
// 	if (!files)
// 	{
// 		perror("malloc failed");
// 		closedir(dp);
// 		return (NULL);
// 	}
// 	i = 0;
// 	j = 0;
// 	entry = readdir(dp);
// 	while (entry)
// 	{
// 		if (entry->d_name[0] != '.')
// 		{
// 			if (i >= capacity)
// 			{
// 				capacity *= 2;
// 				new_files = malloc(sizeof(char *) * capacity);
// 				if (!new_files)
// 				{
// 					perror("malloc for resizing failed");
// 					while (i > 0)
// 						free(files[--i]);
// 					free(files);
// 					closedir(dp);
// 					return (NULL);
// 				}
// 				while (j < i)
// 				{
// 					new_files[j] = files[j];
// 					j++;
// 				}
// 				free(files);
// 				files = new_files;
// 			}
// 			files[i] = strdup(entry->d_name);
// 			if (!files[i])
// 			{
// 				perror("strdup failed");
// 				while (i > 0)
// 					free(files[--i]);
// 				free(files);
// 				closedir(dp);
// 				return (NULL);
// 			}
// 			i++;
// 		}
// 		entry = readdir(dp);
// 	}
// 	files[i] = NULL;
// 	closedir(dp);
// 	return (files);
// }
