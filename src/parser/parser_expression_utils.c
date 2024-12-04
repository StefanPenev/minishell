/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_expression_utils.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stfn <stfn@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/04 20:17:40 by stfn              #+#    #+#             */
/*   Updated: 2024/12/04 20:20:10 by stfn             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include "minishell.h"

t_ast	*handle_parse_error(t_ast *left)
{
	ft_putstr_fd("Error: Expected pipeline after logical operator.\n",
		STDERR_FILENO);
	ast_free(left);
	return (NULL);
}

t_ast	*handle_allocation_error(t_ast *left, t_ast *right)
{
	ast_free(left);
	ast_free(right);
	return (NULL);
}

t_ast_node_type	get_logical_type(t_parser *parser)
{
	if (parser->current_token->type == TOKEN_AND)
		return (AST_LOGICAL_AND);
	return (AST_LOGICAL_OR);
}
