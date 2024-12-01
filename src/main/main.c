/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stfn <stfn@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/09 21:41:55 by stfn              #+#    #+#             */
/*   Updated: 2024/12/01 00:36:21 by stfn             ###   ########.fr       */
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
	if (cmd == NULL || cmd->args == NULL || cmd->args[0] == NULL)
	{
		printf("Error: Invalid command or arguments.\n");
		exit(EXIT_FAILURE);
	}
	while (i < sizeof(builtins) / sizeof(builtins[0]))
	{
		if (ft_strcmp(cmd->args[0], builtins[i]) == 0)
		{
			return (1);
		}
		i++;
	}
	// DO NOT FORGET TO DELETE -G FROM MAKEFILE
	return (0);
}

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
			restore_standard_fds(*saved_stdin, *saved_stdout, *saved_stderr);
			return (-1);
		}
	}
	return (0);
}

// void	execute_ast_command(t_command *cmd, t_shell_context **shell_ctx)
// {
// 	int saved_stdin, saved_stdout, saved_stderr;
// 	if (setup_redirections(cmd, &saved_stdin, &saved_stdout, &saved_stderr) ==
// 		-1)
// 	{
// 		set_exit_status((*shell_ctx)->process, 1);
// 		return ;
// 	}
// 	if (is_builtin(cmd))
// 	{
// 		execute_builtin(cmd, (*shell_ctx)->env_copy, (*shell_ctx)->process);
// 	}
// 	else
// 	{
// 		execute_external_commands(cmd, (*shell_ctx)->env_copy,
// 			(*shell_ctx)->process);
// 	}
// 	restore_standard_fds(saved_stdin, saved_stdout, saved_stderr);
// }
void execute_ast_command(t_command *cmd, t_shell_context **shell_ctx) {
    int saved_stdin, saved_stdout, saved_stderr;

    if (setup_redirections(cmd, &saved_stdin, &saved_stdout, &saved_stderr) == -1)
        return;

    if (is_builtin(cmd)) {
        execute_builtin(cmd, (*shell_ctx)->env_copy, (*shell_ctx)->process);
    } else {
        pid_t pid = fork();
        if (pid == -1) {
            perror("fork");
            restore_standard_fds(saved_stdin, saved_stdout, saved_stderr);
            return;
        }
        if (pid == 0) {
            // Child process
            t_redirection *redir = cmd->redirections;
            while (redir) {
                if (redir->type == HEREDOC) {
                    if (dup2(redir->fd, STDIN_FILENO) == -1) {
                        perror("dup2");
                        exit(EXIT_FAILURE);
                    }
                    close(redir->fd);
                }
                // Handle other input/output redirections if necessary
                redir = redir->next;
            }
            // Execute the external command
            execute_external_commands(cmd, (*shell_ctx)->env_copy, (*shell_ctx)->process);
            exit(EXIT_SUCCESS);
        } else {
            // Parent process
            waitpid(pid, NULL, 0);
        }
    }

    restore_standard_fds(saved_stdin, saved_stdout, saved_stderr);
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

void	assign_vars(t_shell_context **shell_ctx, t_process *process,
		t_env **env_copy, char **envp)
{
	*shell_ctx = malloc(sizeof(t_shell_context));
	if (!*shell_ctx)
	{
		printf("malloc");
		return ;
	}
	process->last_exit_status = 0;
	*env_copy = init_env(envp);
	(*shell_ctx)->process = process;
	(*shell_ctx)->env_copy = *env_copy;
}

int	main(int argc, char **argv, char **envp)
{
	t_term_context	ctx;
	char			*input;
	t_env			*env_copy;
	t_process		process;
	t_shell_context	*shell_ctx;

	shell_ctx = NULL;
	env_copy = NULL;
	(void)argc;
	(void)argv;
	setup_signals(&ctx);
	assign_vars(&shell_ctx, &process, &env_copy, envp);
	// shell_ctx = malloc(sizeof(t_shell_context));
	// process.last_exit_status = 0;
	// env_copy = init_env(envp);
	// shell_ctx->process = &process;
	// shell_ctx->env_copy = env_copy;
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
