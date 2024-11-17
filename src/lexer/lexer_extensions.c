/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_extensions.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stfn <stfn@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/11 21:22:50 by stfn              #+#    #+#             */
/*   Updated: 2024/11/17 18:55:07 by stfn             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"
#include "minishell.h"

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
	while (lexer->current_char && !ft_isspace(lexer->current_char)
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

char	*lexer_expand_variable(t_lexer *lexer, size_t *length)
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
	value = getenv(var_name);
	free(var_name);
	if (value)
		return (ft_strdup(value));
	else
		return (ft_strdup(""));
}
