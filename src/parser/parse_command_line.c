/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_command_line.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stfn <stfn@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/04 20:40:04 by stfn              #+#    #+#             */
/*   Updated: 2024/12/04 21:00:59 by stfn             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include "minishell.h"

static int	add_redirection(t_ast *expr, t_redirection *redir)
{
	t_redirection	*last;

	if (expr->type == AST_COMMAND)
	{
		if (!expr->u_data.command->redirections)
			expr->u_data.command->redirections = redir;
		else
		{
			last = expr->u_data.command->redirections;
			while (last->next)
				last = last->next;
			last->next = redir;
		}
		return (1);
	}
	return (0);
}

static t_ast	*handle_redirection_error(t_ast *expr)
{
	ast_free(expr);
	return (NULL);
}

static t_ast	*handle_invalid_redirection(t_ast *expr, t_redirection *redir)
{
	ft_print_error("Error: Redirection not associated with a command.\n",
		NULL, NULL);
	ast_free(expr);
	free_redirections(redir);
	return (NULL);
}

static t_ast	*handle_parser_redirections(t_parser *parser, t_ast *expr)
{
	t_redirection	*redir;

	while (parser->current_token
		&& is_redirection_token(parser->current_token->type))
	{
		redir = parse_redirection(parser);
		if (!redir)
			return (handle_redirection_error(expr));
		if (!add_redirection(expr, redir))
			return (handle_invalid_redirection(expr, redir));
	}
	return (expr);
}

t_ast	*parse_command_line(t_parser *parser)
{
	t_ast	*expr;

	expr = parse_logical_expression(parser);
	if (!expr)
		return (NULL);
	return (handle_parser_redirections(parser, expr));
}

// Parse a command line
// t_ast	*parse_command_line(t_parser *parser)
// {
// 	t_ast			*expr;
// 	t_redirection	*redir;
// 	t_redirection	*last;

// 	expr = parse_logical_expression(parser);
// 	if (!expr)
// 		return (NULL);
// 	while (parser->current_token
// 		&& is_redirection_token(parser->current_token->type))
// 	{
// 		redir = parse_redirection(parser);
// 		if (!redir)
// 		{
// 			ast_free(expr);
// 			return (NULL);
// 		}
// 		if (expr->type == AST_COMMAND)
// 		{
// 			if (!expr->u_data.command->redirections)
// 			{
// 				expr->u_data.command->redirections = redir;
// 			}
// 			else
// 			{
// 				last = expr->u_data.command->redirections;
// 				while (last->next)
// 					last = last->next;
// 				last->next = redir;
// 			}
// 		}
// 		else
// 		{
// 			ft_print_error("Error: Redirection not associated with a
//             command.\n", NULL, NULL);
// 			ast_free(expr);
// 			free_redirections(redir);
// 			return (NULL);
// 		}
// 	}
// 	return (expr);
// }
