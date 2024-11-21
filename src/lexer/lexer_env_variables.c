/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_env_variables.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stfn <stfn@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/21 19:33:08 by stfn              #+#    #+#             */
/*   Updated: 2024/11/21 20:19:20 by stfn             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"
#include "minishell.h"

char	*lexer_expand_dollar(t_lexer *lexer, t_token *head, t_env *env_copy)
{
	char	*value;
	char	*expanded_value;
	char	*tmp;
	size_t	var_len;

	expanded_value = ft_strdup("");
	if (!expanded_value)
		return (NULL);
	while (lexer->current_char == '$')
	{
		value = lexer_expand_variable(lexer, &var_len, env_copy);
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

char	*lexer_collect_dollar(t_lexer *lexer, t_token *head, t_env *env_copy)
{
	char	*expanded_value;
	char	*tmp;
	char	char_as_str[2];

	expanded_value = lexer_expand_dollar(lexer, head, env_copy);
	if (!expanded_value)
		return (NULL);
	while (lexer->current_char && !ft_isspace(lexer->current_char)
		&& lexer->current_char != '$')
	{
		char_as_str[0] = lexer->current_char;
		char_as_str[1] = '\0';
		tmp = expanded_value;
		expanded_value = ft_strjoin(expanded_value, char_as_str);
		free(tmp);
		lexer->pos++;
		lexer->current_char = lexer->input[lexer->pos];
	}
	return (expanded_value);
}

t_token	*lexer_handle_dollar(t_lexer *lexer, t_token *head, t_env *env_copy)
{
	char	*expanded_value;
	t_token	*new_tok;

	expanded_value = lexer_collect_dollar(lexer, head, env_copy);
	if (!expanded_value)
		return (NULL);
	new_tok = lexer_new_token(TOKEN_WORD, expanded_value);
	free(expanded_value);
	return (new_tok);
}
