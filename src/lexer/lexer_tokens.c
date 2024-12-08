/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_tokens.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stefan <stefan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/18 16:27:07 by stfn              #+#    #+#             */
/*   Updated: 2024/12/08 21:48:49 by stefan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"
#include "minishell.h"

/* 
 * Handles the processing of quoted strings in the lexer. Collects the quoted 
 * string and returns a token of type TOKEN_WORD with the string as its value. 
 * If the quote is unclosed, it returns a TOKEN_ERROR.
 */
t_token	*lexer_handle_quotes(t_lexer *lexer, t_shell_context **shell_ctx)
{
	t_token	*new_tok;
	char	*str;

	str = lexer_collect_quoted(lexer, lexer->current_char, shell_ctx);
	if (!str)
		new_tok = lexer_new_token(TOKEN_ERROR, "Unclosed quote");
	else
	{
		new_tok = lexer_new_token(TOKEN_WORD, str);
		free(str);
	}
	return (new_tok);
}

/* 
 * Handles the processing of regular words in the lexer. Collects the word 
 * and returns a token of type TOKEN_WORD. The word is freed after token 
 * creation.
 */
t_token	*lexer_handle_word(t_lexer *lexer, t_shell_context **shell_ctx)
{
	t_token	*new_tok;
	char	*word;

	new_tok = NULL;
	word = lexer_collect_word(lexer, shell_ctx);
	if (word)
	{
		new_tok = lexer_new_token(TOKEN_WORD, word);
		free(word);
	}
	return (new_tok);
}

/* 
 * Processes a single token based on the current character in the lexer. 
 * Handles special characters, words, and the dollar symbol, returning 
 * the appropriate token or an error message.
 */
t_token	*lexer_process_token(t_lexer *lexer, t_token *head,
	t_shell_context **shell_ctx)
{
	t_token	*new_tok;

	new_tok = NULL;
	if (lexer->current_char == '$')
		new_tok = lexer_handle_dollar(lexer, head, shell_ctx);
	else if (is_special_char(lexer->current_char))
		new_tok = lexer_handle_operator(lexer);
	else if (!is_special_char(lexer->current_char)
		&& !ft_isspace(lexer->current_char))
	{
		new_tok = lexer_handle_word(lexer, shell_ctx);
	}
	else
	{
		printf("Character '%c' is not required to interpret\n",
			lexer->current_char);
		new_tok = lexer_new_token(TOKEN_ERROR, "Invalid character encountered");
		lexer->current_char = '\0';
	}
	return (new_tok);
}

/* 
 * Finalizes the token list by appending a TOKEN_EOF token to indicate 
 * the end of input. Returns the modified token list or NULL on failure.
 */
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
