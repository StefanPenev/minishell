/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_wildcard_utils.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stefan <stefan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/18 10:33:14 by stfn              #+#    #+#             */
/*   Updated: 2024/12/08 21:44:14 by stefan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"
#include "minishell.h"

/* 
 * Frees an array of file pointers and the memory for the array itself. 
 * Iterates through the array, freeing each file pointer before freeing 
 * the array.
 */
void	free_files(char **files, int size)
{
	while (size > 0)
		free(files[--size]);
	free(files);
}

/* 
 * Resizes the files array to accommodate more elements by doubling its 
 * capacity. Preserves existing elements during reallocation. Returns 1 
 * on success or 0 on failure.
 */
int	resize_array(char ***files, int *capacity, int current_size)
{
	char	**new_files;
	int		i;

	new_files = NULL;
	i = 0;
	*capacity *= 2;
	new_files = malloc(sizeof(char *) * (*capacity));
	if (!new_files)
	{
		perror("malloc for resizing failed");
		return (0);
	}
	while (i < current_size)
	{
		new_files[i] = (*files)[i];
		i++;
	}
	free(*files);
	*files = new_files;
	return (1);
}
