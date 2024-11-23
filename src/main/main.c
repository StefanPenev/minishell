/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stfn <stfn@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/09 21:41:55 by stfn              #+#    #+#             */
/*   Updated: 2024/11/23 22:57:38 by stfn             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ast.h"
#include "lexer.h"
#include "minishell.h"
#include "parser.h"
#include "process.h"
#include "signals.h"

void	setup_signals(t_term_context *ctx)
{
	handle_signals();
	disable_echoctl(ctx);
}

char	*read_input(void)
{
	char	*input;

	input = readline("minishell$ ");
	if (!input)
		printf("exit\n");
	else if (*input)
		add_history(input);
	return (input);
}

// Execute buildins
void	execute_builtin(t_command *cmd, t_env *env_copy, t_process *process)//, int pipe_flag)
{
	if (ft_strcmp(cmd->args[0], "pwd") == 0)
		execute_pwd(env_copy, process);
	else if (ft_strcmp(cmd->args[0], "echo") == 0)
		execute_echo(cmd, process);
	else if (strcmp(cmd->args[0], "cd") == 0)
		execute_cd(cmd, env_copy, process);
	else if (strcmp(cmd->args[0], "export") == 0)
		execute_export(cmd, env_copy, process);
	else if (strcmp(cmd->args[0], "unset") == 0)
		execute_unset(cmd, env_copy, process);
	else if (ft_strcmp(cmd->args[0], "env") == 0)
		execute_env(env_copy, process);
	else if (ft_strcmp(cmd->args[0], "exit") == 0)
		execute_exit(cmd, process);
}

int	is_builtin(t_command *cmd)
{
	static const char	*builtins[] = {"echo", "cd", "pwd", "export", "unset",
			"env", "exit"};
	size_t				i;

	i = 0;
	while (i < sizeof(builtins) / sizeof(builtins[0]))
	{
		if (ft_strcmp(cmd->args[0], builtins[i]) == 0)
		{
			return (1); // Command is a built-in
		}
		i++;
	}
	return (0); // Command is not a built-in
}

void	process_command(char *input, t_shell_context **shell_ctx)
{
	t_token		*tokens;
	t_ast		*ast;
	t_command	*cmd;

	tokens = process_lexer(input, shell_ctx);
	if (!tokens)
	{
		lexer_free_tokens(tokens);
		return ;
	}
	ast = process_parser(tokens);
	if (!ast)
	{
		lexer_free_tokens(tokens);
		free(input);
		return ;
	}
	// Execute command
	if (ast->type == AST_COMMAND)
	{
		cmd = ast->u_data.command;
		if (is_builtin(cmd))
			execute_builtin(cmd, (*shell_ctx)->env_copy, (*shell_ctx)->process);//, 0);
		// new added, maybe move somethere
		else if (ft_strncmp(cmd->args[0], "./", 2) == 0)
			execute_external_commands(cmd, (*shell_ctx)->env_copy,
				(*shell_ctx)->process);
		// end
		else
			printf("%s\n", "Execution Test");
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
	t_term_context	ctx;
	char			*input;
	t_env			*env_copy;
	t_process		process;
	t_shell_context	*shell_ctx;

	shell_ctx = NULL;
	(void)argc;
	(void)argv;
	setup_signals(&ctx);
	shell_ctx = malloc(sizeof(t_shell_context));
	process.last_exit_status = 0;
	//process.pipe_flag = 0;
	env_copy = init_env(envp);
	shell_ctx->process = &process;
	shell_ctx->env_copy = env_copy;
	while (1)
	{
		input = read_input();
		if (!input)
			break ;
		process_command(input, &shell_ctx);
	}
	free_shell_ctx(shell_ctx);
	restore_terminal(&ctx);
	return (0);
}
