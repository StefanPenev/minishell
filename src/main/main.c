/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stfn <stfn@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/09 21:41:55 by stfn              #+#    #+#             */
/*   Updated: 2024/12/04 23:15:21 by stfn             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ast.h"
#include "lexer.h"
#include "minishell.h"
#include "parser.h"
#include "process.h"
#include "signals.h"

char	*read_input(void)
{
	char	*input;

	input = readline("minishell$ ");
	if (!input)
		printf("exit\n");
	else if (input && *input)
		add_history(input);
	return (input);
}

void	process_command(char *input, t_shell_context **shell_ctx)
{
	t_token	*tokens;
	t_ast	*ast;

	tokens = process_lexer(input, shell_ctx);
	if (!tokens)
	{
		lexer_free_tokens(tokens);
		return ;
	}
	ast = process_parser(tokens);
	if (!ast)
	{
		free(input);
		return ;
	}
	if (ast->type == AST_COMMAND)
	{
		execute_ast_command(ast->u_data.command, shell_ctx);
	}
	else if (ast->type == AST_PIPELINE)
	{
		main_pipes_process(ast, *shell_ctx);
	}
	ast_free(ast);
	cleanup(tokens, input);
}

int	main(int argc, char **argv, char **envp)
{
	char			*input;
	t_env			*env_copy;
	t_process		process;
	t_shell_context	*shell_ctx;

	shell_ctx = NULL;
	env_copy = NULL;
	(void)argc;
	(void)argv;
	setup_signals();
	assign_vars(&shell_ctx, &process, &env_copy, envp);
	while (1)
	{
		input = read_input();
		if (!input)
			break ;
		process_command(input, &shell_ctx);
	}
	free_shell_ctx(shell_ctx);
	rl_clear_history();
	return (0);
}
