/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stfn <stfn@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/09 21:41:55 by stfn              #+#    #+#             */
/*   Updated: 2024/11/19 19:02:12 by stfn             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "signals.h"
#include "lexer.h"
#include "parser.h"
#include "ast.h"
#include "minishell.h"

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

void	process(char *input, char **envp)
{
	t_token	*tokens;
	t_ast	*ast;

	tokens = process_lexer(input, envp);
	if (!tokens)
		return ;
	ast = process_parser(tokens);
	if (!ast)
	{
		free(input);
		return ;
	}
	//Process/Execute
	cleanup(tokens, input);
}

int	main(int argc, char **argv, char **envp)
{
	t_term_context	ctx;
	char			*input;

	(void)argc;
	(void)argv;
	setup_signals(&ctx);
	while (1)
	{
		input = read_input();
		if (!input)
			break ;
		process(input, envp);
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
