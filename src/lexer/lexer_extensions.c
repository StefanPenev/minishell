/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_extensions.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stfn <stfn@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/11 21:22:50 by stfn              #+#    #+#             */
/*   Updated: 2024/12/05 14:13:25 by stfn             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"
#include "minishell.h"
#include "process.h"

/* Collect a subword (part of a word) */
char	*lexer_collect_subword(t_lexer *lexer, t_shell_context **shell_ctx)
{
	char	*temp_str;
	char	quote_type;
	size_t	start;

	if (lexer->current_char == '"' || lexer->current_char == '\'')
	{
		quote_type = lexer->current_char;
		temp_str = lexer_collect_quoted(lexer, quote_type, shell_ctx);
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

char	*lexer_collect_word(t_lexer *lexer, t_shell_context **shell_ctx)
{
	char	*str;
	char	*new_str;
	char	*temp_str;
	char	quote_type;

	str = ft_strdup("");
	if (!str)
		return (NULL);
	while (lexer->current_char && !ft_isspace(lexer->current_char)
		&& !is_special_char(lexer->current_char))
	{
		if (lexer->current_char == '"' || lexer->current_char == '\'')
		{
			quote_type = lexer->current_char;
			temp_str = lexer_collect_quoted(lexer, quote_type, shell_ctx);
			if (!temp_str)
			{
				free(str);
				return (NULL);
			}
		}
		else
		{
			temp_str = lexer_collect_subword(lexer, shell_ctx);
			if (!temp_str)
			{
				free(str);
				return (NULL);
			}
		}
		new_str = ft_strjoin(str, temp_str);
		free(temp_str);
		free(str);
		if (!new_str)
			return (NULL);
		str = new_str;
	}
	return (str);
}

/* Collect a quoted string (single or double quotes) */
char	*lexer_collect_quoted(t_lexer *lexer, char quote_type,
	t_shell_context **shell_ctx)
{
	char	*expanded_str;
	char	*temp_str;
	char	char_as_str[2];
	char	*tmp;

	lexer_advance(lexer);
	expanded_str = ft_strdup("");
	if (!expanded_str)
		return (NULL);
	while (lexer->current_char && lexer->current_char != quote_type)
	{
		if (quote_type == '"' && lexer->current_char == '$')
		{
			temp_str = lexer_expand_dollar(lexer, NULL, shell_ctx);
			if (!temp_str)
			{
				free(expanded_str);
				return (NULL);
			}
		}
		else
		{
			char_as_str[0] = lexer->current_char;
			char_as_str[1] = '\0';
			temp_str = ft_strdup(char_as_str);
			lexer_advance(lexer);
		}
		tmp = expanded_str;
		expanded_str = ft_strjoin(expanded_str, temp_str);
		free(tmp);
		free(temp_str);
		if (!expanded_str)
			return (NULL);
	}
	if (lexer->current_char != quote_type)
	{
		printf("Error: Unclosed %c quote at position %zu\n", quote_type, lexer->pos);
		free(expanded_str);
		return (NULL);
	}
	lexer_advance(lexer);
	return (expanded_str);
}

char	*lexer_expand_variable(t_lexer *lexer, size_t *length, t_env *env_copy,
	int last_exit_status)
{
	size_t	start;
	char	*var_name;
	char	*value;

	start = lexer->pos + 1; // Position after '$'
	if (lexer->input[start] == '\0')
	{
		*length = 1;
		return (ft_strdup("$"));
	}
	else if (lexer->input[start] == '"' || lexer->input[start] == '\'')
	{
		char	quote_type;
		size_t	end;

		quote_type = lexer->input[start];
		if (lexer->input[start + 1] == '\0')
		{
			*length = 1;
			return (ft_strdup("$"));
		}
		else
		{
			end = start + 1;
			while (lexer->input[end] && lexer->input[end] != quote_type)
				end++;
			if (!lexer->input[end])
			{
				*length = 1;
				return (ft_strdup("$"));
			}
			*length = (end - lexer->pos) + 1;
			size_t quoted_length = end - (start + 1);
			char *quoted_str = ft_strndup(lexer->input + start + 1, quoted_length);
			if (!quoted_str)
				return (NULL);
			char *expanded_str = ft_strdup(quoted_str);
			free(quoted_str);
			return (expanded_str);
		}
	}
	else if (lexer->input[start] == '?')
	{
		*length = 2;
		return (ft_itoa(last_exit_status));
	}
	else if (ft_isdigit(lexer->input[start]))
	{
		*length = 2;
		return (ft_strdup(""));
	}
	else if (ft_isalpha(lexer->input[start]) || lexer->input[start] == '_')
	{
		size_t var_len = 0;
		while (ft_isalnum(lexer->input[start + var_len])
			|| lexer->input[start + var_len] == '_')
			var_len++;
		*length = var_len + 1;
		var_name = ft_strndup(lexer->input + start, var_len);
		if (!var_name)
			return (NULL);
		value = ft_getenv(var_name, env_copy);
		free(var_name);
		if (value)
			return (ft_strdup(value));
		else
			return (ft_strdup(""));
	}
	else
	{
		*length = 1;
		return (ft_strdup("$"));
	}
}
