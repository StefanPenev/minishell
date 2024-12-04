/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_cleanup.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: spenev <spenev@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/04 11:23:18 by spenev            #+#    #+#             */
/*   Updated: 2024/12/04 11:36:45 by spenev           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	cleanup(t_token *tokens, char *input)
{
	lexer_free_tokens(tokens);
	free(input);
}

void	free_shell_ctx(t_shell_context *shell_ctx)
{
	if (!shell_ctx)
		return ;
	if (shell_ctx->env_copy)
	{
		free_env(&(shell_ctx->env_copy));
		shell_ctx->env_copy = NULL;
	}
	free(shell_ctx);
	shell_ctx = NULL;
}
