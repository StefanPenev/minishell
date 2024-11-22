/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stfn <stfn@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/10 00:11:30 by stfn              #+#    #+#             */
/*   Updated: 2024/11/22 10:46:49 by stfn             ###   ########.fr       */
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

static t_token	*lexer_process_loop(t_lexer *lexer,
	t_token **head, t_token **current, t_shell_context **shell_ctx)
{
	t_token	*new_tok;

	while (lexer->current_char)
	{
		if (isspace(lexer->current_char))
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

// t_token	*lexer_tokenize(char *input, char **envp)
// {
// 	t_token	*head;
// 	t_token	*current;
// 	t_token	*new_tok;
// 	t_lexer	lexer = {input, 0, input[0], envp, 0};
// 	char	*value;
// 	size_t	var_len;
// 	char	*str;
// 	char	*word;

// 	head = NULL;
// 	current = NULL;
// 	while (lexer.current_char)
// 	{
// 		if (isspace(lexer.current_char))
// 		{
// 			lexer_skip_whitespace(&lexer);
// 			continue ;
// 		}
// 		new_tok = NULL;
// 		if (lexer.current_char == '$')
// 		{
// 			value = lexer_expand_variable(&lexer, &var_len);
// 			if (!value)
// 			{
// 				lexer_free_tokens(head);
// 				return (NULL);
// 			}
// 			new_tok = lexer_new_token(TOKEN_WORD, value);
// 			free(value);
// 			lexer.pos += var_len;
// 			lexer.current_char = lexer.input[lexer.pos];
// 		}
// 		else if (lexer.current_char == '"' || lexer.current_char == '\'')
// 		{
// 			str = lexer_collect_quoted(&lexer, lexer.current_char);
// 			if (!str)
// 				new_tok = lexer_new_token(TOKEN_ERROR, "Unclosed quote");
// 			else
// 			{
// 				new_tok = lexer_new_token(TOKEN_WORD, str);
// 				free(str);
// 			}
// 		}
// 		else if (is_special_char(lexer.current_char))
// 		{
// 			new_tok = lexer_handle_operator(&lexer);
// 		}
// 		else if (!is_special_char(lexer.current_char)
// 			&& !isspace(lexer.current_char))
// 		{
// 			word = lexer_collect_word(&lexer);
// 			if (word)
// 			{
// 				new_tok = lexer_new_token(TOKEN_WORD, word);
// 				free(word);
// 			}
// 		}
// 		else
// 		{
// 			printf("Character '%c' is not required to interpret\n",
// 				lexer.current_char);
// 			new_tok = lexer_new_token(TOKEN_ERROR,
// 					"Invalid character encountered");
// 			lexer.current_char = '\0';
// 		}
// 		if (!new_tok)
// 		{
// 			lexer_free_tokens(head);
// 			return (NULL);
// 		}
// 		lexer_append_token(&head, &current, new_tok);
// 		if (new_tok->type == TOKEN_ERROR)
// 		{
// 			lexer_free_tokens(head);
// 			return (NULL);
// 		}
// 	}
// 	new_tok = lexer_new_token(TOKEN_EOF, NULL);
// 	if (new_tok)
// 		lexer_append_token(&head, &current, new_tok);
// 	else
// 	{
// 		lexer_free_tokens(head);
// 		return (NULL);
// 	}
// 	return (head);
// }
