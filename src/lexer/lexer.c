/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stfn <stfn@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/10 00:11:30 by stfn              #+#    #+#             */
/*   Updated: 2024/11/12 14:40:49 by stfn             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"

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
	else if (lexer->current_char == '*')
		return (handle_wildcard(lexer));
	return (NULL);
}

t_token	*lexer_tokenize(char *input, char **envp)
{
	t_token	*head;
	t_token	*current;
	t_token	*new_tok;
	t_token	*eof_token;
	char	*str;
	char	*word;
	char	*value;
	char	**wildcard_expansion;
	size_t	var_len;
	int		i;

	if (!input)
		return (NULL);
	head = NULL;
	current = NULL;
	t_lexer lexer = {input, 0, input[0], envp};
	while (lexer.current_char)
	{
		if (isspace(lexer.current_char))
		{
			lexer_skip_whitespace(&lexer);
			continue ;
		}
		new_tok = NULL;
		new_tok = lexer_handle_operator(&lexer);
		if (!new_tok)
		{
			if (lexer.current_char == '"' || lexer.current_char == '\'')
			{
				str = lexer_collect_quoted(&lexer, lexer.current_char);
				if (!str)
				{
					new_tok = lexer_new_token(TOKEN_ERROR, "Unclosed quote");
					if (!new_tok)
					{
						lexer_free_tokens(head);
						return (NULL);
					}
					lexer_append_token(&head, &current, new_tok);
					lexer_free_tokens(head);
					return (NULL);
				}
				new_tok = lexer_new_token(TOKEN_WORD, str);
				free(str);
			}
			else if (lexer.current_char == '$')
			{
				value = lexer_expand_variable(&lexer, &var_len);
				new_tok = lexer_new_token(TOKEN_WORD, value);
				free(value);
				lexer.pos += var_len - 1;
				lexer.current_char = lexer.input[lexer.pos];
			}
			else if (lexer.current_char == '*')
			{
				wildcard_expansion = expand_wildcard();
				if (wildcard_expansion)
				{
					i = 0;
					while (wildcard_expansion[i])
					{
						new_tok = lexer_new_token(TOKEN_WORD,
								wildcard_expansion[i]);
						if (new_tok)
							lexer_append_token(&head, &current, new_tok);
						else
						{
							lexer_free_tokens(head);
							while (wildcard_expansion[i])
							{
								free(wildcard_expansion[i]);
								i++;
							}
							free(wildcard_expansion);
							return (NULL);
						}
						free(wildcard_expansion[i]);
						i++;
					}
					free(wildcard_expansion);
				}
				continue ;
			}
			else
			{
				word = lexer_collect_word(&lexer);
				new_tok = lexer_new_token(TOKEN_WORD, word);
				free(word);
			}
		}
		if (new_tok)
			lexer_append_token(&head, &current, new_tok);
		else
		{
			lexer_free_tokens(head);
			return (NULL);
		}
		if (new_tok->type == TOKEN_ERROR)
		{
			lexer_free_tokens(head);
			return (NULL);
		}
	}
	eof_token = lexer_new_token(TOKEN_EOF, NULL);
	if (eof_token)
		lexer_append_token(&head, &current, eof_token);
	else
	{
		lexer_free_tokens(head);
		return (NULL);
	}
	return (head);
}
