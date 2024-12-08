/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_wildcard.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stefan <stefan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/17 18:55:25 by stfn              #+#    #+#             */
/*   Updated: 2024/12/08 21:42:38 by stefan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"
#include "minishell.h"

/* 
 * Adds a file name to the files array. Resizes the array if needed to 
 * accommodate more files. Returns 1 on success, 0 on failure.
 */
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

/* 
 * Reads directory entries and adds valid file names to the files array. 
 * Skips hidden files (names starting with '.'). Returns 1 on success, 0 
 * on failure.
 */
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

/* 
 * Allocates an initial array of file pointers with a given capacity. 
 * Returns the array on success or NULL on failure.
 */
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

/* 
 * Processes a directory stream, reading entries and storing file names 
 * in an allocated array. Returns the array of file names or NULL on 
 * failure.
 */
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

/* 
 * Expands a wildcard by opening the current directory, processing its 
 * entries, and returning an array of file names. Returns NULL on failure.
 */
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
