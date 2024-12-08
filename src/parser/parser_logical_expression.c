/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_logical_expression.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stefan <stefan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/04 17:58:38 by stfn              #+#    #+#             */
/*   Updated: 2024/12/08 23:06:16 by stefan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include "minishell.h"

/* 
 * Parses the right side of a logical or pipeline expression. If the current 
 * token is a left parenthesis, a parenthesized expression is parsed. Otherwise, 
 * a pipeline expression is parsed.
 */
t_ast	*parse_right_expression(t_parser *parser)
{
	if (parser->current_token
		&& parser->current_token->type == TOKEN_LEFT_PAREN)
		return (parse_parenthesized_expression(parser));
	return (parse_pipeline(parser));
}

/* 
 * Creates a new logical node for an abstract syntax tree (AST). The node type 
 * is set to the specified logical operation (AND/OR). The left and right 
 * children of the node are set to the provided left and right AST nodes.
 */
t_ast	*create_logical_node(t_ast_node_type type, t_ast *left, t_ast *right)
{
	t_ast	*node;

	node = malloc(sizeof(t_ast));
	if (!node)
		return (NULL);
	node->type = type;
	node->u_data.logical.left = left;
	node->u_data.logical.right = right;
	return (node);
}

/* 
 * Parses logical operations (AND/OR) in an expression. Continuously parses 
 * right-hand side expressions while logical operators are encountered. 
 * Constructs logical nodes connecting left and right expressions.
 */
t_ast	*parse_logical_operations(t_parser *parser, t_ast *left)
{
	t_ast			*right;
	t_ast			*node;
	t_ast_node_type	type;

	while (parser->current_token && (parser->current_token->type == TOKEN_AND
			|| parser->current_token->type == TOKEN_OR))
	{
		type = get_logical_type(parser);
		parser_advance(parser);
		right = parse_right_expression(parser);
		if (!right)
			return (handle_parse_error(left));
		node = create_logical_node(type, left, right);
		if (!node)
			return (handle_allocation_error(left, right));
		left = node;
	}
	return (left);
}

/* 
 * Parses the initial part of an expression. If the current token is a left 
 * parenthesis, a parenthesized expression is parsed. Otherwise, a pipeline 
 * expression is parsed.
 */
t_ast	*parse_initial_expression(t_parser *parser)
{
	t_ast	*expr;

	expr = NULL;
	if (parser->current_token
		&& parser->current_token->type == TOKEN_LEFT_PAREN)
		expr = parse_parenthesized_expression(parser);
	else
		expr = parse_pipeline(parser);
	return (expr);
}

/* 
 * Parses a logical expression that may include logical AND/OR operations. It 
 * first parses the initial expression and then processes any logical operations 
 * encountered in the expression. Returns the final AST representing the logical 
 * expression.
 */
t_ast	*parse_logical_expression(t_parser *parser)
{
	t_ast	*left;

	left = parse_initial_expression(parser);
	if (!left)
		return (NULL);
	return (parse_logical_operations(parser, left));
}
