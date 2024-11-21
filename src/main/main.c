/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stfn <stfn@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/09 21:41:55 by stfn              #+#    #+#             */
/*   Updated: 2024/11/21 22:18:55 by stfn             ###   ########.fr       */
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
void	execute_builtin(t_command *cmd, t_env *env_copy, t_process *process)
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

void	process_command(char *input, t_env *env_copy, t_process *process)
{
	t_token		*tokens;
	t_ast		*ast;
	t_command	*cmd;

	tokens = process_lexer(input, env_copy);
	if (!tokens)
		return ;
	ast = process_parser(tokens);
	if (!ast)
	{
		free(input);
		return ;
	}
	// Execute command
	if (ast->type == AST_COMMAND)
	{
		cmd = ast->u_data.command;
		if (is_builtin(cmd))
			execute_builtin(cmd, env_copy, process);
		// new added, maybe move somethere
		else if (ft_strncmp(cmd->args[0], "./", 2) == 0)
			execute_external_commands(cmd, env_copy, process);
		//end
		else
			printf("%s\n", "Execution Test");
	}
	cleanup(tokens, input);
}

int	main(int argc, char **argv, char **envp)
{
	t_term_context	ctx;
	char			*input;
	t_env			*env_copy;
	t_process		process;

	(void)argc;
	(void)argv;
	setup_signals(&ctx);
	process.last_exit_status = 0;
	env_copy = init_env(envp);
	while (1)
	{
		input = read_input();
		if (!input)
			break ;
		process_command(input, env_copy, &process);
	}
	restore_terminal(&ctx);
	return (0);
}

// int	main(int argc, char **argv, char **envp)
// {
// 	char			*input;
// 	t_token			*tokens;
// 	t_term_context	ctx;
// 	t_ast			*ast;

// 	(void)argc;
// 	(void)argv;
// 	handle_signals();
// 	disable_echoctl(&ctx);
// 	while (1)
// 	{
// 		input = readline("minishell$ ");
// 		if (!input)
// 		{
// 			printf("exit\n");
// 			break ;
// 		}
// 		if (*input)
// 			add_history(input);
// 		tokens = lexer_tokenize(input, envp);
// 		if (!tokens)
// 		{
// 			free(input);
// 			continue ;
// 		}
// 		if (tokens->type == TOKEN_ERROR)
// 		{
// 			printf("Error: %s\n", lexer_get_error_message(tokens));
// 			lexer_free_tokens(tokens);
// 			free(input);
// 			continue ;
// 		}
// 		print_tokens(tokens);
// 		ast = parse_tokens(tokens);
// 		if (!ast)
// 		{
// 			fprintf(stderr, "Parsing error\n");
// 			lexer_free_tokens(tokens);
// 			free(input);
// 			continue ;
// 		}
// 		print_ast(ast, 0);

// 		//process

// 		lexer_free_tokens(tokens);
// 		free(input);
// 	}
// 	restore_terminal(&ctx);
// 	return (0);
// }
