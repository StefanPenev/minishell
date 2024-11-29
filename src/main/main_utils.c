/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anilchen <anilchen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/19 18:59:05 by stfn              #+#    #+#             */
/*   Updated: 2024/11/29 17:52:52 by anilchen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "lexer.h"
#include "parser.h"
#include "process.h"

void	cleanup(t_token *tokens, char *input)
{
	lexer_free_tokens(tokens);
	free(input);
}

t_token	*process_lexer(char *input, t_shell_context **shell_ctx)
{
	t_token	*tokens;

	tokens = lexer_tokenize(input, shell_ctx);
	if (!tokens || tokens->type == TOKEN_ERROR)
	{
		if (tokens)
		{
			printf("Error: %s\n", lexer_get_error_message(tokens));
			lexer_free_tokens(tokens);
		}
		free(input);
		return (NULL);
	}
	//print_tokens(tokens);
	return (tokens);
}

t_ast	*process_parser(t_token *tokens)
{
	t_ast	*ast;

	ast = parse_tokens(tokens);
	if (!ast)
	{
		fprintf(stderr, "Parsing error\n");
		lexer_free_tokens(tokens);
		return (NULL);
	}
	//print_ast(ast, 0);
	return (ast);
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
