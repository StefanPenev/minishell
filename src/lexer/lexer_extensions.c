/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_extensions.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stfn <stfn@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/11 21:22:50 by stfn              #+#    #+#             */
/*   Updated: 2024/11/21 20:04:52 by stfn             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"
#include "minishell.h"
#include "process.h"

/* Collect a subword (part of a word) */
char	*lexer_collect_subword(t_lexer *lexer)
{
	char	*temp_str;
	char	quote_type;
	size_t	start;

	if (lexer->current_char == '"' || lexer->current_char == '\'')
	{
		quote_type = lexer->current_char;
		temp_str = lexer_collect_quoted(lexer, quote_type);
	}
	else
	{
		start = lexer->pos;
		while (lexer->current_char && !ft_isspace(lexer->current_char)
			&& !is_special_char(lexer->current_char)
			&& lexer->current_char != '"' && lexer->current_char != '\'')
		{
			lexer_advance(lexer);
		}
		temp_str = ft_strndup(lexer->input + start, lexer->pos - start);
	}
	return (temp_str);
}

/* Collect a word (non-special, non-whitespace characters) */
char	*lexer_collect_word(t_lexer *lexer)
{
	char	*str;
	char	*temp_str;

	str = ft_strdup("");
	while (lexer->current_char && !ft_isspace(lexer->current_char)
		&& !is_special_char(lexer->current_char))
	{
		temp_str = lexer_collect_subword(lexer);
		if (!temp_str)
		{
			free(str);
			return (NULL);
		}
		str = ft_strjoin(str, temp_str);
		free(temp_str);
	}
	return (str);
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

static char	*lexer_get_var_name(t_lexer *lexer, size_t start, size_t *length)
{
	size_t	var_len;

	var_len = 0;
	while (ft_isalnum(lexer->input[start + var_len])
		|| lexer->input[start + var_len] == '_')
		var_len++;
	if (var_len == 0)
	{
		*length = 1;
		return (NULL);
	}
	*length = var_len + 1;
	return (ft_strndup(lexer->input + start, var_len));
}

char	*ft_getenv(char *var_name, t_env *env_copy)
{
	t_env	*current;

	current = env_copy;
	while (current != NULL)
	{
		if (ft_strcmp(current->key, var_name) == 0)
			return (current->value);
		current = current->next;
	}
	return (getenv(var_name));
}

char	*lexer_expand_variable(t_lexer *lexer, size_t *length, t_env *env_copy)
{
	size_t	start;
	char	*var_name;
	char	*value;

	start = lexer->pos + 1;
	if (lexer->input[start] == '?')
	{
		*length = 2;
		return (ft_itoa(lexer->last_exit_status));
	}
	var_name = lexer_get_var_name(lexer, start, length);
	if (!var_name)
		return (ft_strdup("$"));
	value = ft_getenv(var_name, env_copy);
	free(var_name);
	if (value)
		return (ft_strdup(value));
	else
		return (ft_strdup(""));
}
