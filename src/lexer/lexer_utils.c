/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stefan <stefan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/11 18:15:02 by stfn              #+#    #+#             */
/*   Updated: 2024/12/08 21:46:41 by stefan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"
#include "minishell.h"

/* 
 * Checks if a character is a special shell character. Returns 1 if the 
 * character is special, otherwise 0.
 */
int	is_special_char(char c)
{
	return (c == '|' || c == '<' || c == '>' || c == '&' || c == '('
		|| c == ')' || c == '*');
}

/* 
 * Advances the lexer by one character in the input string. Updates the 
 * current position and the current character in the lexer.
 */
void	lexer_advance(t_lexer *lexer)
{
	if (lexer->current_char != '\0')
	{
		lexer->pos++;
		lexer->current_char = lexer->input[lexer->pos];
	}
}

/* 
 * Skips whitespace characters in the input string. Advances the lexer 
 * until a non-whitespace character is encountered.
 */
void	lexer_skip_whitespace(t_lexer *lexer)
{
	while (lexer->current_char && ft_isspace(lexer->current_char))
		lexer_advance(lexer);
}

/* 
 * Retrieves an error message corresponding to a token of type TOKEN_ERROR. 
 * Returns the error message or NULL if the token type is not an error.
 */
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

/* 
 * Frees a linked list of tokens. Releases memory for the value of each 
 * token and the token structures themselves.
 */
void	lexer_free_tokens(t_token *head)
{
	t_token	*current;
	t_token	*next;

	current = head;
	while (current)
	{
		next = current->next;
		if (current->value)
		{
			free(current->value);
			current->value = NULL;
		}
		free(current);
		current = next;
	}
}
