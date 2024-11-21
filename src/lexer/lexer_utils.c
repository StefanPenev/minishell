/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stfn <stfn@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/11 18:15:02 by stfn              #+#    #+#             */
/*   Updated: 2024/11/21 19:10:34 by stfn             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"
#include "minishell.h"

/* Check if a character is a special shell character */
int	is_special_char(char c)
{
	return (c == '|' || c == '<' || c == '>' || c == '$'
		|| c == '&' || c == '(' || c == ')' || c == '*');
}

/* Advance the lexer by one character */
void	lexer_advance(t_lexer *lexer)
{
	if (lexer->current_char != '\0')
	{
		lexer->pos++;
		lexer->current_char = lexer->input[lexer->pos];
	}
}

/* Skip whitespace characters in the input */
void	lexer_skip_whitespace(t_lexer *lexer)
{
	while (lexer->current_char && ft_isspace(lexer->current_char))
		lexer_advance(lexer);
}

/* Utility function to get error message based on token type */
const char	*lexer_get_error_message(t_token *token)
{
	static const char	*error_messages[] = {
	[TOKEN_ERROR]
		= "Lexer encountered an unexpected character or malformed token.",
	};

	if (token->type == TOKEN_ERROR)
		return (error_messages[TOKEN_ERROR]);
	return (NULL);
}

void	lexer_free_tokens(t_token *head)
{
	t_token	*current;
	t_token	*next;

	current = head;
	while (current)
	{
		next = current->next;
		if (current->value)
			free(current->value);
		free(current);
		current = next;
	}
}
