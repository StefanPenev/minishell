/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_quotes.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stefan <stefan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/07 10:09:15 by stfn              #+#    #+#             */
/*   Updated: 2024/12/08 21:49:53 by stefan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"
#include "minishell.h"

/* 
 * Collects a segment of a quoted string. If the quote is a double quote and 
 * the current character is '$', it will expand the dollar sign. Otherwise, 
 * it simply adds the current character to the string.
 */
static char	*lexer_collect_quoted_segment(t_lexer *lexer, char quote_type,
	t_shell_context **shell_ctx)
{
	char	*temp_str;
	char	char_as_str[2];

	if (quote_type == '"' && lexer->current_char == '$')
	{
		temp_str = lexer_expand_dollar(lexer, NULL, shell_ctx);
		if (!temp_str)
			return (NULL);
	}
	else
	{
		char_as_str[0] = lexer->current_char;
		char_as_str[1] = '\0';
		temp_str = ft_strdup(char_as_str);
		if (temp_str)
			lexer_advance(lexer);
	}
	return (temp_str);
}

/* 
 * Processes the content within a quoted string. Collects segments until the 
 * closing quote is found, expanding any dollar signs encountered. Returns 
 * the fully expanded string.
 */
static char	*process_quoted_content(t_lexer *lexer, char quote_type,
	t_shell_context **shell_ctx, char **expanded_str)
{
	char	*temp_str;
	char	*tmp;

	while (lexer->current_char && lexer->current_char != quote_type)
	{
		temp_str = lexer_collect_quoted_segment(lexer, quote_type, shell_ctx);
		if (!temp_str)
		{
			free(*expanded_str);
			return (NULL);
		}
		tmp = *expanded_str;
		*expanded_str = ft_strjoin(*expanded_str, temp_str);
		free(tmp);
		free(temp_str);
		if (!(*expanded_str))
			return (NULL);
	}
	return (*expanded_str);
}

/* 
 * Collects a quoted string from the input, expanding dollar signs if needed. 
 * It ensures the quote is closed and returns the resulting string. If there 
 * is an error (unclosed quote), it returns NULL.
 */
char	*lexer_collect_quoted(t_lexer *lexer, char quote_type,
	t_shell_context **shell_ctx)
{
	char	*expanded_str;

	lexer_advance(lexer);
	expanded_str = ft_strdup("");
	if (!expanded_str)
		return (NULL);
	if (!process_quoted_content(lexer, quote_type, shell_ctx, &expanded_str))
		return (NULL);
	if (lexer->current_char != quote_type)
	{
		printf("Error: Unclosed %c quote at position %zu\n",
			quote_type, lexer->pos);
		free(expanded_str);
		return (NULL);
	}
	lexer_advance(lexer);
	return (expanded_str);
}
