/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_expression.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stfn <stfn@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/13 08:55:59 by stfn              #+#    #+#             */
/*   Updated: 2024/12/04 21:01:19 by stfn             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include "minishell.h"

// t_ast	*parse_pipeline(t_parser *parser)
// {
// 	t_ast	*left;
// 	t_ast	*right;
// 	t_ast	*pipe_node;

// 	left = parse_command(parser);
// 	if (!left)
// 		return (NULL);
// 	while (parser->current_token && parser->current_token->type == TOKEN_PIPE)
// 	{
// 		parser_advance(parser);
// 		right = parse_command(parser);
// 		if (!right)
// 		{
// 			ft_putstr_fd("Error: Expected command after '|'.\n", STDERR_FILENO);
// 			ast_free(left);
// 			return (NULL);
// 		}
// 		pipe_node = malloc(sizeof(t_ast));
// 		if (!pipe_node)
// 		{
// 			ast_free(left);
// 			ast_free(right);
// 			return (NULL);
// 		}
// 		pipe_node->type = AST_PIPELINE;
// 		pipe_node->u_data.pipeline.left = left;
// 		pipe_node->u_data.pipeline.right = right;
// 		left = pipe_node;
// 	}
// 	return (left);
// }
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

t_ast	*parse_pipeline(t_parser *parser)
{
	t_ast	*left;

	left = parse_command(parser);
	if (!left)
		return (NULL);
	return (parse_pipeline_sequence(parser, left));
}

// Parse a parenthesized expression
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

// Main parse function
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
