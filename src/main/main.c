/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: spenev <spenev@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/09 21:41:55 by stfn              #+#    #+#             */
/*   Updated: 2024/12/04 11:57:34 by spenev           ###   ########.fr       */
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