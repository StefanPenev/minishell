/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_extensions.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stefan <stefan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/11 21:22:50 by stfn              #+#    #+#             */
/*   Updated: 2024/12/08 23:17:43 by stefan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"
#include "minishell.h"
#include "process.h"

/* 
 * Processes quoted strings, extracting the content between quotes and 
 * expanding it. Returns the quoted string or "$" if the quote is unclosed.
 */
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

/* 
 * Handles cases where the input is either empty or contains a quote.
 * Returns the expanded string or "$" if the input is empty.
 */
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

/* 
 * Handles special cases like the '?' character (exit status) or digits.
 * Expands the variable accordingly, returning either exit status or empty.
 */
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

/* 
 * Handles variable names by extracting the name, looking it up in the 
 * environment, and returning its value. If not found, returns an empty string.
 */
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

/* 
 * Expands a variable by handling quotes, special cases, and variable names. 
 * Looks up environment variables and returns their value or an empty string.
 */
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
