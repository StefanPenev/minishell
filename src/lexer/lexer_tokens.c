/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_tokens.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stfn <stfn@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/18 16:27:07 by stfn              #+#    #+#             */
/*   Updated: 2024/11/22 10:47:16 by stfn             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"
#include "minishell.h"

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

t_token	*lexer_process_token(t_lexer *lexer, t_token *head, t_shell_context **shell_ctx)
{
	t_token	*new_tok;

	new_tok = NULL;
	if (lexer->current_char == '$')
		new_tok = lexer_handle_dollar(lexer, head, shell_ctx);
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
