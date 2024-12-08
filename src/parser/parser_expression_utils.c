/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_expression_utils.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stefan <stefan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/04 20:17:40 by stfn              #+#    #+#             */
/*   Updated: 2024/12/08 23:09:10 by stefan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include "minishell.h"

/* 
 * Handles parse error when a pipeline is expected after a logical operator. 
 * Prints an error message and frees the left AST node. Returns NULL.
 */
t_ast	*handle_parse_error(t_ast *left)
{
	ft_putstr_fd("Error: Expected pipeline after logical operator.\n",
		STDERR_FILENO);
	ast_free(left);
	return (NULL);
}

/* 
 * Handles memory allocation error by freeing both left and right AST nodes. 
 * Returns NULL after freeing the allocated nodes.
 */
t_ast	*handle_allocation_error(t_ast *left, t_ast *right)
{
	ast_free(left);
	ast_free(right);
	return (NULL);
}

/* 
 * Determines the type of logical operator based on the current token. 
 * If the token is "AND", returns AST_LOGICAL_AND, otherwise returns AST_LOGICAL_OR.
 */
t_ast_node_type	get_logical_type(t_parser *parser)
{
	if (parser->current_token->type == TOKEN_AND)
		return (AST_LOGICAL_AND);
	return (AST_LOGICAL_OR);
}
