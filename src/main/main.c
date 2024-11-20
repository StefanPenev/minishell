/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: spenev <spenev@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/09 21:41:55 by stfn              #+#    #+#             */
/*   Updated: 2024/11/20 12:54:24 by spenev           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "signals.h"
#include "lexer.h"
#include "parser.h"
#include "ast.h"
#include "minishell.h"
#include "process.h"

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

////////////////////////////////
int	minishell_pwd(t_env *env_copy, t_process *process)
{
	t_env	*current;

	current = env_copy;
	// pwd shouldnt have any arguments, but if they are, it still works
	while (current != NULL)
	{
		// printf("Key: %s, Value: %s\n", current->key, current->value);
		if (strcmp(current->key, "PWD") == 0)
		{
			write(1, current->value, ft_strlen(current->value));
			write(1, "\n", 1);
			set_exit_status(process, 0);
			return (EXIT_SUCCESS);
		}
		current = current->next;
	}
	set_exit_status(process, 0);
	return (EXIT_SUCCESS);
}

//Execute buildins
void	execute_builtin(t_command *cmd, t_env *env_copy, t_process *process)
{
	if (strcmp(cmd->args[0], "pwd") == 0)
		minishell_pwd(env_copy, process);
	// if (strcmp(cmd->args[0], "echo") == 0)
	// 	execute_echo(cmd);
	// else if (strcmp(cmd->args[0], "cd") == 0)
	// 	execute_cd(cmd);
	// else if (strcmp(cmd->args[0], "pwd") == 0)
	// 	execute_pwd();
	// else if (strcmp(cmd->args[0], "export") == 0)
	// 	execute_export(cmd);
	// else if (strcmp(cmd->args[0], "unset") == 0)
	// 	execute_unset(cmd);
	// else if (strcmp(cmd->args[0], "env") == 0)
	// 	execute_env(envp);
	// else if (strcmp(cmd->args[0], "exit") == 0)
	// 	execute_exit(cmd);
}

int is_builtin(t_command *cmd) {
    static const char *builtins[] = {"echo", "cd", "pwd", "export", "unset", "env", "exit"};
    for (size_t i = 0; i < sizeof(builtins) / sizeof(builtins[0]); i++) {
        if (strcmp(cmd->args[0], builtins[i]) == 0) {
            return 1; // Command is a built-in
        }
    }
    return 0; // Command is not a built-in
}
/////////////////////////////////

void	process_command(char *input, t_env *env_copy, t_process process, char **envp)
{
	t_token		*tokens;
	t_ast		*ast;
	t_command	*cmd;

	tokens = process_lexer(input, envp);
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
			execute_builtin(cmd, env_copy, &process);
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
		process_command(input, env_copy, process, envp);
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
