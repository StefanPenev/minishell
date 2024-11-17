/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_wildcard.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stfn <stfn@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/17 18:55:25 by stfn              #+#    #+#             */
/*   Updated: 2024/11/17 20:39:01 by stfn             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"
#include "minishell.h"

char	**expand_wildcard(void)
{
	int				i;
	int				j;
	int				capacity;
	char			**files;
	struct dirent	*entry;
	DIR				*dp;
	char			**new_files;

	dp = opendir(".");
	if (!dp)
	{
		perror("opendir failed");
		return (NULL);
	}
	capacity = INITIAL_CAPACITY;
	files = malloc(sizeof(char *) * capacity);
	if (!files)
	{
		perror("malloc failed");
		closedir(dp);
		return (NULL);
	}
	i = 0;
	j = 0;
	entry = readdir(dp);
	while (entry)
	{
		if (entry->d_name[0] != '.')
		{
			if (i >= capacity)
			{
				capacity *= 2;
				new_files = malloc(sizeof(char *) * capacity);
				if (!new_files)
				{
					perror("malloc for resizing failed");
					while (i > 0)
						free(files[--i]);
					free(files);
					closedir(dp);
					return (NULL);
				}
				while (j < i)
				{
					new_files[j] = files[j];
					j++;
				}
				free(files);
				files = new_files;
			}
			files[i] = strdup(entry->d_name);
			if (!files[i])
			{
				perror("strdup failed");
				while (i > 0)
					free(files[--i]);
				free(files);
				closedir(dp);
				return (NULL);
			}
			i++;
		}
		entry = readdir(dp);
	}
	files[i] = NULL;
	closedir(dp);
	return (files);
}
