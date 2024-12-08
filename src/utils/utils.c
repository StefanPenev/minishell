/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stefan <stefan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/11 21:13:20 by stfn              #+#    #+#             */
/*   Updated: 2024/12/08 23:01:21 by stefan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* 
 * Duplicate a string up to a specified number of characters (n). It allocates 
 * memory for the new string, copies the characters, and adds a null-terminator 
 * at the end. If the allocation fails, NULL is returned.
 */
char	*ft_strndup(const char *s, size_t n)
{
	char			*res;
	unsigned int	i;

	i = 0;
	res = malloc(sizeof(char) * (n + 1));
	if (res == NULL)
		return (NULL);
	while (i < n)
	{
		res[i] = s[i];
		i++;
	}
	res[i] = '\0';
	return (res);
}

/* 
 * Concatenate the source string (src) to the destination string (dest).
 * The function appends the characters of src to the end of dest and ensures
 * that dest is null-terminated. It returns the destination string.
 */
char	*ft_strcat(char *dest, char *src)
{
	int	i;
	int	j;

	i = 0;
	while (dest[i] != '\0')
		i++;
	j = 0;
	while (src[j] != '\0')
	{
		dest[i + j] = src[j];
		j++;
	}
	dest[i + j] = '\0';
	return (dest);
}

/* 
 * Reallocate memory for a given pointer (ptr) to a new size (new_size).
 * If new_size is 0, it frees the pointer and returns NULL. If ptr is NULL,
 * it behaves like malloc and allocates the requested size. Otherwise, it 
 * copies the old data to the new memory, frees the old memory, and returns 
 * the new pointer.
 */
void	*ft_realloc(void *ptr, size_t new_size)
{
	void	*new_ptr;

	if (new_size == 0)
	{
		free(ptr);
		return (NULL);
	}
	if (!ptr)
		return (malloc(new_size));
	new_ptr = malloc(new_size);
	if (!new_ptr)
		return (NULL);
	ft_memcpy(new_ptr, ptr, new_size);
	free(ptr);
	return (new_ptr);
}

/* 
 * Check if a character (c) is a whitespace character. This includes space, 
 * tab, newline, carriage return, and vertical tab. The function returns 
 * a non-zero value if true, otherwise 0.
 */
int	ft_isspace(int c)
{
	return ((c >= 9 && c <= 13) || c == 32);
}

/* 
 * Copy the source string (src) to the destination string (dest). The function
 * iterates through each character in src and copies it to dest until the 
 * null-terminator is encountered, at which point it appends a null-terminator 
 * at the end of dest.
 */
char	*ft_strcpy(char *dest, char *src)
{
	int	i;

	i = 0;
	while (src[i] != '\0')
	{
		dest[i] = src[i];
		i++;
	}
	dest[i] = '\0';
	return (dest);
}
