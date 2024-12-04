/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: spenev <spenev@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/09 21:41:55 by stfn              #+#    #+#             */
/*   Updated: 2024/12/04 11:02:17 by spenev           ###   ########.fr       */
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
	return (0);
}

void	restore_standard_fds(int saved_stdin, int saved_stdout,
		int saved_stderr)
{
	if (saved_stdin != -1 && isatty(saved_stdin))
	{
		if (dup2(saved_stdin, STDIN_FILENO) == -1)
		{
			perror("Failed to restore stdin");
		}
		close(saved_stdin);
	}
	if (saved_stdout != -1 && isatty(saved_stdout))
	{
		if (dup2(saved_stdout, STDOUT_FILENO) == -1)
		{
			perror("Failed to restore stdout");
		}
		close(saved_stdout);
	}
	if (saved_stderr != -1 && isatty(saved_stderr))
	{
		if (dup2(saved_stderr, STDERR_FILENO) == -1)
		{
			perror("Failed to restore stderr");
		}
		close(saved_stderr);
	}
}

int	setup_redirections(t_command *cmd, int *saved_stdin, int *saved_stdout,
		int *saved_stderr)
{
	if (cmd->redirections)
	{
		*saved_stdin = dup(STDIN_FILENO);
		*saved_stdout = dup(STDOUT_FILENO);
		*saved_stderr = dup(STDERR_FILENO);
		if (*saved_stdin == -1 || *saved_stdout == -1 || *saved_stderr == -1)
		{
			perror("dup");
			return (-1);
		}
		if (handle_redirections(cmd) == -1)
		{
			restore_standard_fds(*saved_stdin, *saved_stdout, *saved_stderr);
			return (-1);
		}
		return (1);
	}
	else
	{
		*saved_stdin = -1;
		*saved_stdout = -1;
		*saved_stderr = -1;
		return (0);
	}
}


void	execute_ast_command(t_command *cmd, t_shell_context **shell_ctx)
{
	int				saved_stdin;
	int				saved_stdout;
	int				saved_stderr;
	pid_t			pid;
	t_redirection	*redir;
	int				requires_fork;
	int				redirections_set;

	redirections_set = 0;
	requires_fork = 0;
	redir = cmd->redirections;
	while (redir)
	{
		if (redir->type == HEREDOC)
		{
			requires_fork = 1;
			break ;
		}
		redir = redir->next;
	}
	redirections_set = setup_redirections(cmd, &saved_stdin, &saved_stdout,
			&saved_stderr);
	if (redirections_set == -1)
	{
		set_exit_status((*shell_ctx)->process, 1);
		return ;
	}
	// if (setup_redirections(cmd, &saved_stdin, &saved_stdout,
	//		&saved_stderr) ==
	// 	-1)
	// 	return ;
	if (is_builtin(cmd) && !requires_fork && !redirections_set)
	{
		execute_builtin(cmd, (*shell_ctx)->env_copy, (*shell_ctx)->process);
		restore_standard_fds(saved_stdin, saved_stdout, saved_stderr);
		return ;
	}
	if (!requires_fork && !redirections_set)
	{
		execute_external_commands(cmd, (*shell_ctx)->env_copy,
			(*shell_ctx)->process);
		restore_standard_fds(saved_stdin, saved_stdout, saved_stderr);
		return ;
	}
	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		restore_standard_fds(saved_stdin, saved_stdout, saved_stderr);
		return ;
	}
	if (pid == 0)
	{
		redir = cmd->redirections;
		while (redir)
		{
			if (redir->type == HEREDOC)
			{
				if (dup2(redir->fd, STDIN_FILENO) == -1)
				{
					perror("dup2");
					exit(EXIT_FAILURE);
				}
				close(redir->fd);
			}
			redir = redir->next;
		}
		execute_external_commands(cmd, (*shell_ctx)->env_copy,
			(*shell_ctx)->process);
		exit(EXIT_SUCCESS);
	}
	else
	{
		handle_child_exit_status(pid, (*shell_ctx)->process);
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

// void	execute_ast_command(t_command *cmd, t_shell_context **shell_ctx)
// {
// 	int				saved_stdin;
// 	int				saved_stdout;
// 	int				saved_stderr;
// 	pid_t			pid;
// 	t_redirection	*redir;

// 	printf("[DEBUG] execute_ast_command start: Process pointer: %p,
//		Last exit status: %d\n",
// 		(void *)(*shell_ctx)->process, (*shell_ctx)->process->last_exit_status);
// 	if (setup_redirections(cmd, &saved_stdin, &saved_stdout, &saved_stderr) ==
// 		-1)
// 		return ;
// 	if (is_builtin(cmd))
// 		execute_builtin(cmd, (*shell_ctx)->env_copy, (*shell_ctx)->process);
// 	else
// 	{
// 		pid = fork();
// 		if (pid == -1)
// 		{
// 			perror("fork");
// 			restore_standard_fds(saved_stdin, saved_stdout, saved_stderr);
// 			return ;
// 		}
// 		if (pid == 0)
// 		{
// 			redir = cmd->redirections;
// 			while (redir)
// 			{
// 				if (redir->type == HEREDOC)
// 				{
// 					if (dup2(redir->fd, STDIN_FILENO) == -1)
// 					{
// 						perror("dup2");
// 						exit(EXIT_FAILURE);
// 					}
// 					close(redir->fd);
// 				}
// 				redir = redir->next;
// 			}
// 			printf("[DEBUG] execute_ast_command before calling execute_external_commands: Process pointer:
//				%p, Last exit status: %d\n",
// 			(void *)(*shell_ctx)->process,
//				(*shell_ctx)->process->last_exit_status);
// 			execute_external_commands(cmd, (*shell_ctx)->env_copy,
// 				(*shell_ctx)->process);
// 			printf("[DEBUG] execute_ast_command after calling execute_external_commands: Process pointer:
//				%p, Last exit status: %d\n",
// 			(void *)(*shell_ctx)->process,
//				(*shell_ctx)->process->last_exit_status);
// 			exit(EXIT_SUCCESS);
// 		}
// 		else
// 			handle_child_exit_status(pid, (*shell_ctx)->process);
// 	//waitpid(pid, NULL, 0);
// 	}
// 	printf("[DEBUG] before restore_standard_fds: Process pointer: %p,
//		Last exit status: %d\n",
//     (void *)(*shell_ctx)->process, (*shell_ctx)->process->last_exit_status);
// 	restore_standard_fds(saved_stdin, saved_stdout, saved_stderr);
// 	printf("[DEBUG] execute_ast_command end: Process pointer: %p,
//		Last exit status: %d\n",
//     (void *)(*shell_ctx)->process, (*shell_ctx)->process->last_exit_status);
// }