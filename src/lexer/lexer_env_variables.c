/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_env_variables.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stfn <stfn@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/21 19:33:08 by stfn              #+#    #+#             */
/*   Updated: 2024/12/05 14:14:14 by stfn             ###   ########.fr       */
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

// char	*lexer_expand_dollar(t_lexer *lexer, t_token *head,
// 	t_shell_context **shell_ctx)
// {
// 	char	*value;
// 	char	*expanded_value;
// 	char	*tmp;
// 	size_t	var_len;

// 	expanded_value = ft_strdup("");
// 	if (!expanded_value)
// 		return (NULL);
// 	while (lexer->current_char == '$')
// 	{
// 		value = lexer_expand_variable(lexer, &var_len, (*shell_ctx)->env_copy,
// 				(*shell_ctx)->process->last_exit_status);
// 		if (!value)
// 		{
// 			lexer_free_tokens(head);
// 			free(expanded_value);
// 			return (NULL);
// 		}
// 		tmp = expanded_value;
// 		expanded_value = ft_strjoin(expanded_value, value);
// 		free(tmp);
// 		free(value);
// 		lexer->pos += var_len;
// 		lexer->current_char = lexer->input[lexer->pos];
// 	}
// 	return (expanded_value);
// }

// Helper function to process a single variable and append its value.
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

// Main function orchestrating the expansion process.
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
// char	*lexer_expand_dollar(t_lexer *lexer, t_token *head,
//      t_shell_context **shell_ctx)
// {
//     char	*expanded_value;

//     expanded_value = ft_strdup("");
//     if (!expanded_value)
//         return (NULL);
//     while (lexer->current_char == '$')
//     {
//         // Check next character to prevent consuming quotes
//         if (lexer->input[lexer->pos + 1] == '"' || lexer->input[lexer->pos + 1] == '\'')
//         {
//             // Advance past the '$' to prevent infinite loop
//             lexer_advance(lexer);
//             break;
//         }
//         expanded_value = lexer_process_variable(lexer, expanded_value,
//                 shell_ctx, head);
//         if (!expanded_value)
//             return (NULL);
//     }
//     return (expanded_value);
// }

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
