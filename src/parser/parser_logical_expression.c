/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_logical_expression.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stfn <stfn@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/04 17:58:38 by stfn              #+#    #+#             */
/*   Updated: 2024/12/04 20:21:02 by stfn             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include "minishell.h"

t_ast	*parse_right_expression(t_parser *parser)
{
	if (parser->current_token
		&& parser->current_token->type == TOKEN_LEFT_PAREN)
		return (parse_parenthesized_expression(parser));
	return (parse_pipeline(parser));
}

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

t_ast	*parse_logical_expression(t_parser *parser)
{
	t_ast	*left;

	left = parse_initial_expression(parser);
	if (!left)
		return (NULL);
	return (parse_logical_operations(parser, left));
}

// t_ast	*parse_logical_expression(t_parser *parser)
// {
// 	t_ast			*left;
// 	t_ast			*right;
// 	t_ast			*node;
// 	t_ast_node_type	type;

// 	left = NULL;
// 	if (parser->current_token
// 		&& parser->current_token->type == TOKEN_LEFT_PAREN)
// 		left = parse_parenthesized_expression(parser);
// 	else
// 		left = parse_pipeline(parser);
// 	if (!left)
// 		return (NULL);
// 	while (parser->current_token && (parser->current_token->type == TOKEN_AND
// 			|| parser->current_token->type == TOKEN_OR))
// 	{
// 		if (parser->current_token->type == TOKEN_AND)
// 			type = AST_LOGICAL_AND;
// 		else
// 			type = AST_LOGICAL_OR;
// 		parser_advance(parser);
// 		right = NULL;
// 		if (parser->current_token
// 			&& parser->current_token->type == TOKEN_LEFT_PAREN)
// 			right = parse_parenthesized_expression(parser);
// 		else
// 			right = parse_pipeline(parser);
// 		if (!right)
// 		{
// 			ft_putstr_fd("Error: Expected pipeline after logical operator.\n",
// 				STDERR_FILENO);
// 			ast_free(left);
// 			return (NULL);
// 		}
// 		node = malloc(sizeof(t_ast));
// 		if (!node)
// 		{
// 			ast_free(left);
// 			ast_free(right);
// 			return (NULL);
// 		}
// 		node->type = type;
// 		node->u_data.logical.left = left;
// 		node->u_data.logical.right = right;
// 		left = node;
// 	}
// 	return (left);
// }