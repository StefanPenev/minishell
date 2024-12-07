/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_words.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stfn <stfn@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/07 10:09:23 by stfn              #+#    #+#             */
/*   Updated: 2024/12/07 10:13:25 by stfn             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"
#include "minishell.h"

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

char	*lexer_collect_next_segment(t_lexer *lexer, t_shell_context **shell_ctx)
{
	char	*temp_str;
	char	quote_type;

	if (lexer->current_char == '"' || lexer->current_char == '\'')
	{
		quote_type = lexer->current_char;
		temp_str = lexer_collect_quoted(lexer, quote_type, shell_ctx);
	}
	else
	{
		temp_str = lexer_collect_subword(lexer, shell_ctx);
	}
	return (temp_str);
}

char	*lexer_collect_word(t_lexer *lexer, t_shell_context **shell_ctx)
{
	char	*str;
	char	*new_str;
	char	*temp_str;

	str = ft_strdup("");
	if (!str)
		return (NULL);
	while (lexer->current_char && !ft_isspace(lexer->current_char)
		&& !is_special_char(lexer->current_char))
	{
		temp_str = lexer_collect_next_segment(lexer, shell_ctx);
		if (!temp_str)
		{
			free(str);
			return (NULL);
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
