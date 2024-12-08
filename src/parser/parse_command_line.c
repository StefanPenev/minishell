/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_command_line.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stefan <stefan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/04 20:40:04 by stfn              #+#    #+#             */
/*   Updated: 2024/12/08 23:16:40 by stefan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include "minishell.h"

/* 
 * Adds a redirection to the given expression if the expression is of type 
 * AST_COMMAND. If the command already has redirections, the new redirection 
 * is appended to the end of the list. Returns 1 on success, 0 if the 
 * expression is not a valid command.
 */
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

/* 
 * Handles an error when a redirection is invalid or cannot be added to the 
 * expression. It frees the expression node and returns NULL.
 */
static t_ast	*handle_redirection_error(t_ast *expr)
{
	ast_free(expr);
	return (NULL);
}

/* 
 * Handles the case where a redirection is not associated with a command 
 * expression. Prints an error message, frees the expression and redirection 
 * resources, and returns NULL.
 */
static t_ast	*handle_invalid_redirection(t_ast *expr, t_redirection *redir)
{
	ft_print_error("Error: Redirection not associated with a command.\n",
		NULL, NULL);
	ast_free(expr);
	free_redirections(redir);
	return (NULL);
}

/* 
 * Processes redirections in the given expression. If a redirection token is 
 * found, it parses the redirection and tries to add it to the expression. If 
 * adding the redirection fails, an error handling function is called. Returns 
 * the expression if successful, or NULL on error.
 */
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

/* 
 * Main parsing function for the command line. It starts by parsing logical 
 * expressions and then processes any redirections in the expression. Returns 
 * the fully parsed AST for the command line, or NULL on error.
 */
t_ast	*parse_command_line(t_parser *parser)
{
	t_ast	*expr;

	expr = parse_logical_expression(parser);
	if (!expr)
		return (NULL);
	return (handle_parser_redirections(parser, expr));
}
