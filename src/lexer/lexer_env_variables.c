/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_env_variables.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stefan <stefan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/21 19:33:08 by stfn              #+#    #+#             */
/*   Updated: 2024/12/08 22:46:22 by stefan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"
#include "minishell.h"

/* 
 * Custom getenv function that looks for a variable in the environment list.
 * If not found, it falls back to the standard getenv function.
 */
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

/* 
 * Helper function to process a single variable expansion and append its value
 * to the expanded_value string.
 */
static char	*lexer_process_variable(t_lexer *lexer, char *expanded_value,
	t_shell_context **shell_ctx, t_token *head)
{
	char	*value;
	char	*tmp;
	size_t	var_len;

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
	return (expanded_value);
}

/* 
 * Main function that orchestrates the expansion of variables (starting with $).
 * It collects variable values and appends them to the expanded_value string.
 */
char	*lexer_expand_dollar(t_lexer *lexer, t_token *head,
	t_shell_context **shell_ctx)
{
	char	*expanded_value;

	expanded_value = ft_strdup("");
	if (!expanded_value)
		return (NULL);
	while (lexer->current_char == '$')
	{
		expanded_value = lexer_process_variable(lexer, expanded_value,
				shell_ctx, head);
		if (!expanded_value)
			return (NULL);
	}
	return (expanded_value);
}

/* 
 * Collects the expanded value of a dollar sign, handling variable names,
 * quotes, and other special cases in the input.
 */
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
		if (lexer->current_char == '"')
		{
			printf("Error: Unclosed \" quote at position %zu\n", lexer->pos);
			free(expanded_value);
			return (NULL);
		}
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

/* 
 * Handles the expansion of a dollar sign by calling lexer_collect_dollar
 * and creating a new token for the expanded value.
 */
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
