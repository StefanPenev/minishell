/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_expression.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stfn <stfn@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/13 08:55:59 by stfn              #+#    #+#             */
/*   Updated: 2024/12/03 22:19:03 by stfn             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

t_ast	*parse_pipeline(t_parser *parser)
{
	t_ast	*left;
	t_ast	*right;
	t_ast	*pipe_node;

	left = parse_command(parser);
	if (!left)
		return (NULL);
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
		left = pipe_node;
	}
	return (left);
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

t_ast	*parse_logical_expression(t_parser *parser)
{
	t_ast			*left;
	t_ast			*right;
	t_ast			*node;
	t_ast_node_type	type;

	left = NULL;
	if (parser->current_token
		&& parser->current_token->type == TOKEN_LEFT_PAREN)
		left = parse_parenthesized_expression(parser);
	else
		left = parse_pipeline(parser);
	if (!left)
		return (NULL);
	while (parser->current_token && (parser->current_token->type == TOKEN_AND
			|| parser->current_token->type == TOKEN_OR))
	{
		if (parser->current_token->type == TOKEN_AND)
			type = AST_LOGICAL_AND;
		else
			type = AST_LOGICAL_OR;
		parser_advance(parser);
		right = NULL;
		if (parser->current_token
			&& parser->current_token->type == TOKEN_LEFT_PAREN)
			right = parse_parenthesized_expression(parser);
		else
			right = parse_pipeline(parser);
		if (!right)
		{
			ft_putstr_fd("Error: Expected pipeline after logical operator.\n",
				STDERR_FILENO);
			ast_free(left);
			return (NULL);
		}
		node = malloc(sizeof(t_ast));
		if (!node)
		{
			ast_free(left);
			ast_free(right);
			return (NULL);
		}
		node->type = type;
		node->u_data.logical.left = left;
		node->u_data.logical.right = right;
		left = node;
	}
	return (left);
}

// Parse a command line
t_ast	*parse_command_line(t_parser *parser)
{
	t_ast			*expr;
	t_redirection	*redir;
	t_redirection	*last;

	expr = parse_logical_expression(parser);
	if (!expr)
		return (NULL);
	while (parser->current_token
		&& is_redirection_token(parser->current_token->type))
	{
		redir = parse_redirection(parser);
		if (!redir)
		{
			ast_free(expr);
			return (NULL);
		}
		if (expr->type == AST_COMMAND)
		{
			if (!expr->u_data.command->redirections)
			{
				expr->u_data.command->redirections = redir;
			}
			else
			{
				last = expr->u_data.command->redirections;
				while (last->next)
					last = last->next;
				last->next = redir;
			}
		}
		else
		{
			ft_print_error("Error: Redirection not associated with a command.\n", NULL, NULL);
			ast_free(expr);
			free_redirections(redir);
			return (NULL);
		}
	}
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
