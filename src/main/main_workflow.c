/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_workflow.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stfn <stfn@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/04 11:43:07 by spenev            #+#    #+#             */
/*   Updated: 2024/12/04 23:33:51 by stfn             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "parser.h"

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
