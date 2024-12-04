/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_env_variables.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: spenev <spenev@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/21 19:33:08 by stfn              #+#    #+#             */
/*   Updated: 2024/12/04 10:34:49 by spenev           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"
#include "minishell.h"

char	*ft_getenv(char *var_name, t_env *env_copy)
{
	t_env	*current;

	current = env_copy;
	while (current != NULL)
	{
		if (ft_strcmp(current->key, var_name) == 0)
			return (current->value);
		current = current->next;
	}
	return (getenv(var_name));
}

char	*lexer_expand_dollar(t_lexer *lexer, t_token *head,
	t_shell_context **shell_ctx)
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
		value = lexer_expand_variable(lexer, &var_len, (*shell_ctx)->env_copy,
				(*shell_ctx)->process->last_exit_status);
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

char	*lexer_collect_dollar(t_lexer *lexer, t_token *head,
	t_shell_context **shell_ctx)
{
	char	*expanded_value;
	char	*tmp;
	char	char_as_str[2];

	expanded_value = lexer_expand_dollar(lexer, head, shell_ctx);
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

t_token	*lexer_handle_dollar(t_lexer *lexer, t_token *head,
	t_shell_context **shell_ctx)
{
	char	*expanded_value;
	t_token	*new_tok;

	expanded_value = lexer_collect_dollar(lexer, head, shell_ctx);
	if (!expanded_value)
		return (NULL);
	new_tok = lexer_new_token(TOKEN_WORD, expanded_value);
	free(expanded_value);
	return (new_tok);
}
