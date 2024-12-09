/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: spenev <spenev@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/10 00:11:30 by stfn              #+#    #+#             */
/*   Updated: 2024/12/09 10:13:43 by spenev           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"
#include "minishell.h"

/* 
 * Create a new token with the specified type and value. 
 * Allocates memory for a new token structure, initializes its type and
 * value, and sets the next pointer to NULL.
 */
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

/* 
 * Append a token to the end of a linked list. 
 * Updates the head if the list is empty, or adds the token to the end of the
 * list and moves the current pointer to the newly added token.
 */
void	lexer_append_token(t_token **head, t_token **current, t_token *new_tok)
{
	if (!*head)
		*head = new_tok;
	else
		(*current)->next = new_tok;
	*current = new_tok;
}

/* 
 * Handle operators in the input and dispatch to specific functions based on the
 * current character. Returns a new token corresponding to the operator or NULL
 * if no operator is matched.
 */
t_token	*lexer_handle_operator(t_lexer *lexer)
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
	{
		lexer_advance(lexer);
		return (lexer_new_token(TOKEN_LEFT_PAREN, "("));
	}
	else if (lexer->current_char == ')')
	{
		lexer_advance(lexer);
		return (lexer_new_token(TOKEN_RIGHT_PAREN, ")"));
	}
	else if (lexer->current_char == '*')
		return (handle_wildcard(lexer));
	return (NULL);
}

/* 
 * Main loop for processing characters in the input and generating tokens. 
 * Skips whitespace, processes each token, and appends it to the token list. 
 * Returns the head of the token list or NULL if an error occurs.
 */
static t_token	*lexer_process_loop(t_lexer *lexer,
	t_token **head, t_token **current, t_shell_context **shell_ctx)
{
	t_token	*new_tok;

	while (lexer->current_char)
	{
		if (ft_isspace(lexer->current_char))
		{
			lexer_skip_whitespace(lexer);
			continue ;
		}
		new_tok = lexer_process_token(lexer, *head, shell_ctx);
		if (!new_tok)
		{
			lexer_free_tokens(*head);
			return (NULL);
		}
		lexer_append_token(head, current, new_tok);
		if (new_tok->type == TOKEN_ERROR)
		{
			lexer_free_tokens(*head);
			return (NULL);
		}
	}
	return (*head);
}

/* 
 * Tokenize the given input string into a linked list of tokens. 
 * Initializes the lexer state, processes the input via a loop, 
 * and finalizes the token list. Returns the head of the token list or NULL
 * on failure.
 */
t_token	*lexer_tokenize(char *input, t_shell_context **shell_ctx)
{
	t_token	*head;
	t_token	*current;
	t_lexer	lexer;

	lexer.input = input;
	lexer.pos = 0;
	lexer.current_char = input[0];
	lexer.envp = (*shell_ctx)->env_copy;
	lexer.last_exit_status = 0;
	head = NULL;
	current = NULL;
	if (!lexer_process_loop(&lexer, &head, &current, shell_ctx))
		return (NULL);
	return (lexer_finalize_tokens(head, &current));
}
