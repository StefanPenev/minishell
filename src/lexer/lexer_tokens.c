/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_tokens.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stfn <stfn@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/18 16:27:07 by stfn              #+#    #+#             */
/*   Updated: 2024/11/21 19:11:28 by stfn             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"
#include "minishell.h"

char	*lexer_collect_dollar(t_lexer *lexer, t_token *head)
{
	char	*value;
	char	*expanded_value;
	size_t	var_len;
	char	*tmp;

	expanded_value = ft_strdup("");
	if (!expanded_value)
		return (NULL);
	while (lexer->current_char == '$')
	{
		value = lexer_expand_variable(lexer, &var_len);
		if (!value)
		{
			lexer_free_tokens(head);
			free(expanded_value);
			return (NULL);
		}
		tmp = expanded_value;
		expanded_value = ft_strjoin(expanded_value, value);
		free(tmp);
		free(value);
		lexer->pos += var_len;
		lexer->current_char = lexer->input[lexer->pos];
	}
	return (expanded_value);
}

t_token	*lexer_handle_dollar(t_lexer *lexer, t_token *head)
{
	char	*expanded_value;
	t_token	*new_tok;

	expanded_value = lexer_collect_dollar(lexer, head);
	if (!expanded_value)
		return (NULL);
	new_tok = lexer_new_token(TOKEN_WORD, expanded_value);
	free(expanded_value);
	return (new_tok);
}

t_token	*lexer_handle_quotes(t_lexer *lexer)
{
	t_token	*new_tok;
	char	*str;

	str = lexer_collect_quoted(lexer, lexer->current_char);
	if (!str)
		new_tok = lexer_new_token(TOKEN_ERROR, "Unclosed quote");
	else
	{
		new_tok = lexer_new_token(TOKEN_WORD, str);
		free(str);
	}
	return (new_tok);
}

t_token	*lexer_handle_word(t_lexer *lexer)
{
	t_token	*new_tok;
	char	*word;

	word = lexer_collect_word(lexer);
	if (word)
	{
		new_tok = lexer_new_token(TOKEN_WORD, word);
		free(word);
		return (new_tok);
	}
	return (NULL);
}

t_token	*lexer_process_token(t_lexer *lexer, t_token *head)
{
	t_token	*new_tok;

	new_tok = NULL;
	if (lexer->current_char == '$')
		new_tok = lexer_handle_dollar(lexer, head);
	else if (lexer->current_char == '"' || lexer->current_char == '\'')
		new_tok = lexer_handle_quotes(lexer);
	else if (is_special_char(lexer->current_char))
		new_tok = lexer_handle_operator(lexer);
	else if (!is_special_char(lexer->current_char)
		&& !isspace(lexer->current_char))
		new_tok = lexer_handle_word(lexer);
	else
	{
		printf("Character '%c' is not required to interpret\n",
			lexer->current_char);
		new_tok = lexer_new_token(TOKEN_ERROR,
				"Invalid character encountered");
		lexer->current_char = '\0';
	}
	return (new_tok);
}

t_token	*lexer_finalize_tokens(t_token *head, t_token **current)
{
	t_token	*new_tok;

	new_tok = lexer_new_token(TOKEN_EOF, NULL);
	if (new_tok)
		lexer_append_token(&head, current, new_tok);
	else
	{
		lexer_free_tokens(head);
		return (NULL);
	}
	return (head);
}
