/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_extensions.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stfn <stfn@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/11 21:22:50 by stfn              #+#    #+#             */
/*   Updated: 2024/11/12 00:14:59 by stfn             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"

/* Create a new token */
t_token	*lexer_new_token(t_token_type type, char *value)
{
	t_token	*token;

	token = malloc(sizeof(t_token));
	if (!token)
		return (NULL);
	token->type = type;
	if (value)
		token->value = ft_strdup(value);
	else
		token->value = NULL;
	token->next = NULL;
	return (token);
}

/* Collect a word (non-special, non-whitespace characters) */
char	*lexer_collect_word(t_lexer *lexer)
{
	size_t	start;
	size_t	length;

	start = lexer->pos;
	while (lexer->current_char && !isspace(lexer->current_char)
		&& !is_special_char(lexer->current_char))
		lexer_advance(lexer);
	length = lexer->pos - start;
	return (ft_strndup(lexer->input + start, length));
}

/* Collect a quoted string (single or double quotes) */
char	*lexer_collect_quoted(t_lexer *lexer, char quote_type)
{
	char	*str;
	size_t	length;
	size_t	start;

	lexer_advance(lexer);
	start = lexer->pos;
	while (lexer->current_char && lexer->current_char != quote_type)
		lexer_advance(lexer);
	if (lexer->current_char != quote_type)
	{
		printf("Error: Unclosed %c quote at position %zu\n",
			quote_type, lexer->pos);
		return (NULL);
	}
	length = lexer->pos - start;
	str = ft_strndup(lexer->input + start, length);
	lexer_advance(lexer);
	return (str);
}

/* Expand environment variables */
char	*lexer_expand_variable(t_lexer *lexer, size_t *length)
{
	size_t	start;
	size_t	var_len;
	char	*value;
	char	*var_name;

	start = lexer->pos + 1;
	var_len = 0;
	while (isalnum(lexer->input[start + var_len])
		|| lexer->input[start + var_len] == '_')
		var_len++;
	if (var_len == 0)
	{
		*length = 1;
		return (ft_strdup("$"));
	}
	var_name = ft_strndup(lexer->input + start, var_len);
	value = getenv(var_name);
	free(var_name);
	*length = var_len + 1;
	if (value)
		return (ft_strdup(value));
	else
		return (ft_strdup(""));
}

// /* Handle wildcard expansion manually (for current working directory only) */
// char	**expand_wildcard(void);
// {
// 	int				i;
// 	char			**files;
// 	struct dirent	*entry;
// 	DIR				*dp;

// 	dp = opendir(".");
// 	if (!dp)
// 		return (NULL);
// 	files = malloc(sizeof(char *) * 256);
// 	i = 0;
// 	while ((entry = readdir(dp)))
// 	{
// 		if (entry->d_name[0] != '.')
// 		{
// 			files[i] = ft_strdup(entry->d_name);
// 			i++;
// 		}
// 	}
// 	files[i] = NULL;
// 	closedir(dp);
// 	return (files);
// }

char	**expand_wildcard(void)
{
	int				i;
	char			**files;
	struct dirent	*entry;
	DIR				*dp;

	dp = opendir(".");
	if (!dp)
		return (NULL);
	files = malloc(sizeof(char *) * 256);
	if (!files)
	{
		closedir(dp);
		return (NULL);
	}
	i = 0;
	entry = readdir(dp);
	while (entry)
	{
		if (entry->d_name[0] != '.')
		{
			files[i] = ft_strdup(entry->d_name);
			if (!files[i])
			{
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
