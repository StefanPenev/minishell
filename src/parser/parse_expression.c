/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_expression.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: spenev <spenev@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/13 08:55:59 by stfn              #+#    #+#             */
/*   Updated: 2024/12/09 09:39:44 by spenev           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include "minishell.h"

/* 
 * Creates a new AST node for a pipeline expression with the given left and 
 * right child nodes. If memory allocation fails, it frees the child nodes 
 * and returns NULL.
 */
t_ast	*create_pipe_node(t_ast *left, t_ast *right)
{
	t_ast	*pipe_node;

	pipe_node = malloc(sizeof(t_ast));
	if (!pipe_node)
	{
		ast_free(left);
		ast_free(right);
		return (NULL);
	}
	pipe_node->type = AST_PIPELINE;
	pipe_node->u_data.pipeline.left = left;
	pipe_node->u_data.pipeline.right = right;
	return (pipe_node);
}

/* 
 * Parses a sequence of pipeline commands, connecting them into a chain of AST 
 * nodes. For each pipeline operator '|', a new AST node is created. If a 
 * command is missing after the operator, an error message is displayed.
 */
t_ast	*parse_pipeline_sequence(t_parser *parser, t_ast *left)
{
	t_ast	*right;

	while (parser->current_token && parser->current_token->type == TOKEN_PIPE)
	{
		parser_advance(parser);
		right = parse_command(parser);
		if (!right)
		{
			ft_putstr_fd("Error: Expected command after '|'.\n", STDERR_FILENO);
			ast_free(left);
			return (NULL);
		}
		left = create_pipe_node(left, right);
		if (!left)
			return (NULL);
	}
	return (left);
}

/* 
 * Parses a pipeline by parsing the first command and then processing any 
 * subsequent pipeline sequences. If parsing fails at any step, NULL is returned.
 */
t_ast	*parse_pipeline(t_parser *parser)
{
	t_ast	*left;

	left = parse_command(parser);
	if (!left)
		return (NULL);
	return (parse_pipeline_sequence(parser, left));
}

/* 
 * Parses an expression inside parentheses. If the parentheses are properly 
 * opened and closed and the enclosed expression is valid, it returns the AST 
 * node for the expression. Otherwise, it prints an error message and returns
 * NULL.
 */
t_ast	*parse_parenthesized_expression(t_parser *parser)
{
	t_ast	*expr;

	if (!parser->current_token || parser->current_token->type
		!= TOKEN_LEFT_PAREN)
		return (NULL);
	parser_advance(parser);
	expr = parse_logical_expression(parser);
	if (!expr)
	{
		ft_putstr_fd("Error: Invalid expression inside parentheses.\n",
			STDERR_FILENO);
		return (NULL);
	}
	if (!parser->current_token || parser->current_token->type
		!= TOKEN_RIGHT_PAREN)
	{
		ft_putstr_fd("Error: Expected ')' after expression.\n", STDERR_FILENO);
		ast_free(expr);
		return (NULL);
	}
	parser_advance(parser);
	return (expr);
}

/* 
 * Parses the entire sequence of tokens into an Abstract Syntax Tree (AST). 
 * It starts parsing from the command line and checks if the end of the input 
 * is reached. If an unexpected token or end-of-input is found, an error is 
 * displayed.
 */
t_ast	*parse_tokens(t_token *tokens)
{
	t_ast		*ast;
	t_parser	parser;

	parser.current_token = tokens;
	ast = parse_command_line(&parser);
	if (!parser.current_token || parser.current_token->type != TOKEN_EOF)
	{
		if (parser.current_token && parser.current_token->value)
			ft_print_error("Error: Unexpected ",
				parser.current_token->value, " at end of input.\n");
		else
			ft_print_error("Error: Unexpected end of input.\n", NULL, NULL);
		ast_free(ast);
		return (NULL);
	}
	return (ast);
}
