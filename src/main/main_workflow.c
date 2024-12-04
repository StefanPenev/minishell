/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_workflow.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anilchen <anilchen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/04 11:43:07 by spenev            #+#    #+#             */
/*   Updated: 2024/12/04 15:30:07 by anilchen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "parser.h"

void	execute_ast_command(t_command *cmd, t_shell_context **shell_ctx)
{
	t_fd_backup		fd_backup;
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
	redirections_set = setup_redirections(cmd, &fd_backup, (*shell_ctx)->process);
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
		restore_standard_fds(&fd_backup);
		return ;
	}
	if (!requires_fork && !redirections_set)
	{
		execute_external_commands(cmd, (*shell_ctx)->env_copy,
			(*shell_ctx)->process);
		restore_standard_fds(&fd_backup);
		return ;
	}
	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		restore_standard_fds(&fd_backup);
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
	restore_standard_fds(&fd_backup);
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
	else if (ft_strcmp(cmd->args[0], "cd") == 0)
		execute_cd(cmd, env_copy, process);
	else if (ft_strcmp(cmd->args[0], "export") == 0)
		execute_export(cmd, env_copy, process);
	else if (ft_strcmp(cmd->args[0], "unset") == 0)
		execute_unset(cmd, env_copy, process);
	else if (ft_strcmp(cmd->args[0], "env") == 0 || ft_strcmp(cmd->args[0],
			"/usr/bin/env") == 0)
		execute_env(env_copy, process);
	else if (ft_strcmp(cmd->args[0], "exit") == 0)
		execute_exit(cmd, process);
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
	return (ast);
}

// t_token	*process_lexer(char *input, t_shell_context **shell_ctx)
// {
// 	t_token	*tokens;

// 	tokens = lexer_tokenize(input, shell_ctx);
// 	if (!tokens || tokens->type == TOKEN_ERROR)
// 	{
// 		if (tokens)
// 		{
// 			printf("Error: %s\n", lexer_get_error_message(tokens));
// 			lexer_free_tokens(tokens);
// 		}
// 		free(input);
// 		return (NULL);
// 	}
// 	print_tokens(tokens);
// 	return (tokens);
// }

// t_ast	*process_parser(t_token *tokens)
// {
// 	t_ast	*ast;

// 	ast = parse_tokens(tokens);
// 	if (!ast)
// 	{
// 		fprintf(stderr, "Parsing error\n");
// 		lexer_free_tokens(tokens);
// 		return (NULL);
// 	}
// 	print_ast(ast, 0);
// 	return (ast);
// }
