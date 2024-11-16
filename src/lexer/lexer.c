/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stfn <stfn@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/10 00:11:30 by stfn              #+#    #+#             */
/*   Updated: 2024/11/17 00:23:00 by stfn             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"
#include "minishell.h"

/* Append token to the linked list */
void	lexer_append_token(t_token **head, t_token **current, t_token *new_tok)
{
	if (!*head)
		*head = new_tok;
	else
		(*current)->next = new_tok;
	*current = new_tok;
}

/* Main function to handle operators, dispatches to helper functions */
static t_token	*lexer_handle_operator(t_lexer *lexer)
{
	if (lexer->current_char == '|')
		return (handle_pipe(lexer));
	else if (lexer->current_char == '&')
		return (handle_ampersand(lexer));
	else if (lexer->current_char == '<')
		return (handle_redirect_in(lexer));
	else if (lexer->current_char == '>')
		return (handle_redirect_out(lexer));
	else if (lexer->current_char == '(')
		return (handle_lparen(lexer));
	else if (lexer->current_char == ')')
		return (handle_rparen(lexer));
	else if (lexer->current_char == '*')
		return (handle_wildcard(lexer));
	return (NULL);
}

t_token	*lexer_tokenize(char *input, char **envp) {
	t_token	*head = NULL;
	t_token	*current = NULL;
	t_token	*new_tok;
	t_lexer lexer = {input, 0, input[0], envp, 0}; // Initialize lexer with last_exit_status = 0
	char	*value;
	size_t	var_len;

	while (lexer.current_char) {
		if (isspace(lexer.current_char)) {
			lexer_skip_whitespace(&lexer);
			continue;
		}

		new_tok = NULL;

		if (lexer.current_char == '$') {
			// Expand environment variable or special case like "$?"
			value = lexer_expand_variable(&lexer, &var_len);
			if (!value) {
				lexer_free_tokens(head);
				return (NULL);
			}
			new_tok = lexer_new_token(TOKEN_WORD, value);
			free(value);
			lexer.pos += var_len; // Advance lexer position
			lexer.current_char = lexer.input[lexer.pos];
		} else if (lexer.current_char == '"' || lexer.current_char == '\'') {
			// Handle quoted strings
			char *str = lexer_collect_quoted(&lexer, lexer.current_char);
			if (!str) {
				new_tok = lexer_new_token(TOKEN_ERROR, "Unclosed quote");
			} else {
				new_tok = lexer_new_token(TOKEN_WORD, str);
				free(str);
			}
		} else if (is_special_char(lexer.current_char)) {
			// Handle special characters/operators like |, &, *, etc.
			new_tok = lexer_handle_operator(&lexer);
		} else if (!is_special_char(lexer.current_char) && !isspace(lexer.current_char)) {
			// Handle regular words
			char *word = lexer_collect_word(&lexer);
			if (word) {
				new_tok = lexer_new_token(TOKEN_WORD, word);
				free(word);
			}
		} else {
			// Handle unknown or invalid characters
			char err_msg[50];
			snprintf(err_msg, sizeof(err_msg), "Character '%c' is not required to interpret", lexer.current_char);
			new_tok = lexer_new_token(TOKEN_ERROR, err_msg);
			lexer.current_char = '\0'; // Stop parsing on error
		}

		// Handle error in token creation
		if (!new_tok) {
			lexer_free_tokens(head);
			return (NULL);
		}

		// Append the token to the token list
		lexer_append_token(&head, &current, new_tok);

		// If there's an error token, free everything and return NULL
		if (new_tok->type == TOKEN_ERROR) {
			lexer_free_tokens(head);
			return (NULL);
		}
	}

	// Add EOF token at the end
	new_tok = lexer_new_token(TOKEN_EOF, NULL);
	if (new_tok) {
		lexer_append_token(&head, &current, new_tok);
	} else {
		lexer_free_tokens(head);
		return (NULL);
	}
	return (head);
}
