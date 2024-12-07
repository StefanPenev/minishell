/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_extensions.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stfn <stfn@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/11 21:22:50 by stfn              #+#    #+#             */
/*   Updated: 2024/12/07 16:35:08 by stfn             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"
#include "minishell.h"
#include "process.h"

static char	*process_quotes(t_lexer *lexer, size_t *length, size_t start)
{
	size_t	end;
	size_t	quoted_length;
	char	quote_type;
	char	*quoted_str;
	char	*expanded_str;

	end = start + 1;
	quote_type = lexer->input[start];
	while (lexer->input[end] && lexer->input[end] != quote_type)
		end++;
	if (!lexer->input[end])
	{
		*length = 1;
		return (ft_strdup("$"));
	}
	*length = (end - lexer->pos) + 1;
	quoted_length = end - (start + 1);
	quoted_str = ft_strndup(lexer->input + start + 1, quoted_length);
	if (!quoted_str)
		return (NULL);
	expanded_str = ft_strdup(quoted_str);
	free(quoted_str);
	return (expanded_str);
}

static char	*handle_empty_or_quote(t_lexer *lexer, size_t *length, size_t start)
{
	if (lexer->input[start] == '\0')
	{
		*length = 1;
		return (ft_strdup("$"));
	}
	if (lexer->input[start] == '"' || lexer->input[start] == '\'')
		return (process_quotes(lexer, length, start));
	return (NULL);
}

static char	*handle_special_cases(t_lexer *lexer, size_t *length, size_t start,
	int last_exit_status)
{
	if (lexer->input[start] == '?')
	{
		*length = 2;
		return (ft_itoa(last_exit_status));
	}
	if (ft_isdigit(lexer->input[start]))
	{
		*length = 2;
		return (ft_strdup(""));
	}
	return (NULL);
}

static char	*handle_variable_name(t_lexer *lexer, size_t *length, size_t start,
	t_env *env_copy)
{
	size_t	var_len;
	char	*value;
	char	*var_name;

	var_len = 0;
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
	return (ft_strdup(""));
}

char	*lexer_expand_variable(t_lexer *lexer, size_t *length, t_env *env_copy,
	int last_exit_status)
{
	size_t	start;
	char	*result;

	start = lexer->pos + 1;
	result = handle_empty_or_quote(lexer, length, start);
	if (result)
		return (result);
	result = handle_special_cases(lexer, length, start, last_exit_status);
	if (result)
		return (result);
	if (ft_isalpha(lexer->input[start]) || lexer->input[start] == '_')
		return (handle_variable_name(lexer, length, start, env_copy));
	*length = 1;
	return (ft_strdup("$"));
}

// char	*lexer_expand_variable(t_lexer *lexer, size_t *length, t_env *env_copy,
// 	int last_exit_status)
// {
// 	size_t	start;
// 	char	*var_name;
// 	char	*value;

// 	start = lexer->pos + 1;
// 	if (lexer->input[start] == '\0')
// 	{
// 		*length = 1;
// 		return (ft_strdup("$"));
// 	}
// 	else if (lexer->input[start] == '"' || lexer->input[start] == '\'')
// 	{
// 		char	quote_type;
// 		size_t	end;

// 		quote_type = lexer->input[start];
// 		if (lexer->input[start + 1] == '\0')
// 		{
// 			*length = 1;
// 			return (ft_strdup("$"));
// 		}
// 		else
// 		{
// 			end = start + 1;
// 			while (lexer->input[end] && lexer->input[end] != quote_type)
// 				end++;
// 			if (!lexer->input[end])
// 			{
// 				*length = 1;
// 				return (ft_strdup("$"));
// 			}
// 			*length = (end - lexer->pos) + 1;
// 			size_t quoted_length = end - (start + 1);
// 			char *quoted_str = ft_strndup(lexer->input + start + 1,
//				quoted_length);
// 			if (!quoted_str)
// 				return (NULL);
// 			char *expanded_str = ft_strdup(quoted_str);
// 			free(quoted_str);
// 			return (expanded_str);
// 		}
// 	}
// 	else if (lexer->input[start] == '?')
// 	{
// 		*length = 2;
// 		return (ft_itoa(last_exit_status));
// 	}
// 	else if (ft_isdigit(lexer->input[start]))
// 	{
// 		*length = 2;
// 		return (ft_strdup(""));
// 	}
// 	else if (ft_isalpha(lexer->input[start]) || lexer->input[start] == '_')
// 	{
// 		size_t var_len = 0;
// 		while (ft_isalnum(lexer->input[start + var_len])
// 			|| lexer->input[start + var_len] == '_')
// 			var_len++;
// 		*length = var_len + 1;
// 		var_name = ft_strndup(lexer->input + start, var_len);
// 		if (!var_name)
// 			return (NULL);
// 		value = ft_getenv(var_name, env_copy);
// 		free(var_name);
// 		if (value)
// 			return (ft_strdup(value));
// 		else
// 			return (ft_strdup(""));
// 	}
// 	else
// 	{
// 		*length = 1;
// 		return (ft_strdup("$"));
// 	}
// }
