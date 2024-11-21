/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stfn <stfn@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/19 18:59:05 by stfn              #+#    #+#             */
/*   Updated: 2024/11/21 22:15:55 by stfn             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "lexer.h"
#include "parser.h"

void	cleanup(t_token *tokens, char *input)
{
	lexer_free_tokens(tokens);
	free(input);
}

t_token	*process_lexer(char *input, t_env *envp)
{
	t_token	*tokens;

	tokens = lexer_tokenize(input, envp);
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
	print_tokens(tokens);
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
	print_ast(ast, 0);
	return (ast);
}
