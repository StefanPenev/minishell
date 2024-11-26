/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anilchen <anilchen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/09 21:41:55 by stfn              #+#    #+#             */
/*   Updated: 2024/11/26 14:32:42 by anilchen         ###   ########.fr       */
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
	else if (input && *input)
		add_history(input);
	return (input);
}

// Execute buildins
void	execute_builtin(t_command *cmd, t_env *env_copy, t_process *process)
{
	if (ft_strcmp(cmd->args[0], "pwd") == 0 || ft_strcmp(cmd->args[0],
			"/bin/pwd") == 0)
		execute_pwd(env_copy, process);
	else if (ft_strcmp(cmd->args[0], "echo") == 0 || ft_strcmp(cmd->args[0],
			"/usr/bin/echo") == 0 || ft_strcmp(cmd->args[0], "/bin/echo") == 0)
		execute_echo(cmd, process);
	else if (strcmp(cmd->args[0], "cd") == 0)
		execute_cd(cmd, env_copy, process);
	else if (strcmp(cmd->args[0], "export") == 0)
		execute_export(cmd, env_copy, process);
	else if (strcmp(cmd->args[0], "unset") == 0)
		execute_unset(cmd, env_copy, process);
	else if (ft_strcmp(cmd->args[0], "env") == 0 || ft_strcmp(cmd->args[0],
			"/usr/bin/env") == 0)
		execute_env(env_copy, process);
	else if (ft_strcmp(cmd->args[0], "exit") == 0)
		execute_exit(cmd, process);
}

int	is_builtin(t_command *cmd)
{
	static const char	*builtins[] = {"echo", "cd", "pwd", "export", "unset",
			"env", "exit", "/usr/bin/echo", "/bin/pwd", "/usr/bin/env",
			"/bin/echo"};
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
	// DO NOT FORGET TO DELETE -G FROM MAKEFILE
	return (0); // Command is not a built-in
}

// t_ast	*parse_input(char *input, t_shell_context **shell_ctx)
// {
// 	t_token	*tokens;
// 	t_ast	*ast;

// 	tokens = process_lexer(input, shell_ctx);
// 	if (!tokens)
// 	{
// 		lexer_free_tokens(tokens);
// 		return (NULL);
// 	}
// 	ast = process_parser(tokens);
// 	cleanup(tokens, input);
// 	if (!ast)
// 	{
// 		free(input);
// 		return (NULL);
// 	}
// 	return (ast);
// }

void	restore_standard_fds(int saved_stdin, int saved_stdout,
		int saved_stderr)
{
	dup2(saved_stdin, STDIN_FILENO);
	dup2(saved_stdout, STDOUT_FILENO);
	dup2(saved_stderr, STDERR_FILENO);
	close(saved_stdin);
	close(saved_stdout);
	close(saved_stderr);
}

int	setup_redirections(t_command *cmd, int *saved_stdin, int *saved_stdout,
		int *saved_stderr)
{
	*saved_stdin = dup(STDIN_FILENO);
	*saved_stdout = dup(STDOUT_FILENO);
	*saved_stderr = dup(STDERR_FILENO);
	if (*saved_stdin == -1 || *saved_stdout == -1 || *saved_stderr == -1)
	{
		perror("dup");
		return (-1);
	}
	if (cmd->redirections)
	{
		if (handle_redirections(cmd) == -1)
		{
			write(2, "Error handling redirections\n", 28); //delete later
			restore_standard_fds(*saved_stdin, *saved_stdout, *saved_stderr);
			return (-1);
		}
	}
	return (0);
}

void	execute_ast_command(t_command *cmd, t_shell_context **shell_ctx)
{
	int saved_stdin, saved_stdout, saved_stderr;
	if (setup_redirections(cmd, &saved_stdin, &saved_stdout, &saved_stderr) ==
		-1)
		return ;
	if (is_builtin(cmd))
	{
		execute_builtin(cmd, (*shell_ctx)->env_copy, (*shell_ctx)->process);
	}
	else
	{
		execute_external_commands(cmd, (*shell_ctx)->env_copy,
			(*shell_ctx)->process);
	}
	restore_standard_fds(saved_stdin, saved_stdout, saved_stderr);
}

// void	process_command(char *input, t_shell_context **shell_ctx)
// {
// 	t_ast	*ast;

// 	ast = parse_input(input, shell_ctx);
// 	if (!ast)
// 		return ;
// 	if (ast->type == AST_COMMAND)
// 	{
// 		execute_ast_command(ast->u_data.command, shell_ctx);
// 	}
// 	else if (ast->type == AST_PIPELINE)
// 	{
// 		main_pipes_process(ast, *shell_ctx);
// 	}
// 	ast_free(ast);
// }

void	process_command(char *input, t_shell_context **shell_ctx)
{
	t_token		*tokens;
	t_ast		*ast;
	// t_command	*cmd;
	// int			saved_stdin;
	// int			saved_stdout;
	// int			saved_stderr;

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
	rl_clear_history();
	restore_terminal(&ctx);
	return (0);
}

// For test purposes to isolate lexer and parser from execution
// void test_lexer_parser(char *input, t_shell_context **shell_ctx)
// {
//     t_token *tokens;
//     t_ast *ast;

//     tokens = process_lexer(input, shell_ctx);
//     if (!tokens)
//     {
//         free(input);
//         return ;
//     }
//     ast = process_parser(tokens);
//     if (!ast)
//     {
//         lexer_free_tokens(tokens);
//         free(input);
//         return ;
//     }
//     // Free the AST node and tokens
//     ast_free(ast);
//     lexer_free_tokens(tokens);
//     free(input);
// }

// int main(int argc, char **argv, char **envp)
// {
//     t_term_context ctx;
//     char *input;
//     t_env *env_copy;
//     t_shell_context *shell_ctx;

//     shell_ctx = NULL;
//     (void)argc;
//     (void)argv;
//     setup_signals(&ctx);
//     shell_ctx = malloc(sizeof(t_shell_context));
//     env_copy = init_env(envp);
//     shell_ctx->env_copy = env_copy;
//     while (1)
//     {
//         input = read_input();
//         if (!input)
//             break ;
//         test_lexer_parser(input, &shell_ctx);
//     }
//     free_shell_ctx(shell_ctx);
//     restore_terminal(&ctx);
//     return (0);
// }

////////////////////////////////////////////////////////////////////
// just saved it to be sure, delete after testing
// void	process_command(char *input, t_shell_context **shell_ctx)
// {
// 	t_token		*tokens;
// 	t_ast		*ast;
// 	t_command	*cmd;
// 	int			saved_stdin;
// 	int			saved_stdout;
// 	int			saved_stderr;

// 	tokens = process_lexer(input, shell_ctx);
// 	if (!tokens)
// 	{
// 		lexer_free_tokens(tokens);
// 		return ;
// 	}
// 	ast = process_parser(tokens);
// 	if (!ast)
// 	{
// 		free(input);
// 		return ;
// 	}
// 	// Execute command
// 	if (ast->type == AST_COMMAND)
// 	{
// 		cmd = ast->u_data.command;
// 		saved_stdin = dup(STDIN_FILENO);
// 		saved_stdout = dup(STDOUT_FILENO);
// 		saved_stderr = dup(STDERR_FILENO);
// 		if (saved_stdin == -1 || saved_stdout == -1 || saved_stderr == -1)
// 			perror("dup");
// 		if (cmd->redirections)
// 		{
// 			if (handle_redirections(cmd) == -1)
// 			{
// 				printf("Error handling redirections\n");
// 				dup2(saved_stdin, STDIN_FILENO);
// 				dup2(saved_stdout, STDOUT_FILENO);
// 				dup2(saved_stderr, STDERR_FILENO);
// 				close(saved_stdin);
// 				close(saved_stdout);
// 				close(saved_stderr);
// 				return ;
// 			}
// 		}
// 		if (is_builtin(cmd))
// 			execute_builtin(cmd, (*shell_ctx)->env_copy, (*shell_ctx)->process);
// 		// new added, maybe move somethere
// 		else if (ft_strncmp(cmd->args[0], "./", 2) == 0 || is_builtin(cmd) == 0)
// 			execute_external_commands(cmd, (*shell_ctx)->env_copy,
// 				(*shell_ctx)->process);
// 		// else if ((is_builtin(cmd) == 0))
// 		// {
// 		// 	execute_external_commands(cmd, (*shell_ctx)->env_copy,
// 		// 		(*shell_ctx)->process);
// 		// }
// 		// end
// 		else
// 			printf("%s\n", "Execution Test");
// 		dup2(saved_stdin, STDIN_FILENO);
// 		dup2(saved_stdout, STDOUT_FILENO);
// 		dup2(saved_stderr, STDERR_FILENO);
// 		close(saved_stdin);
// 		close(saved_stdout);
// 		close(saved_stderr);
// 	}
// 	else if (ast->type == AST_PIPELINE)
// 	{
// 		main_pipes_process(ast, *shell_ctx);
// 	}
// 	ast_free(ast);
// 	cleanup(tokens, input);
// }