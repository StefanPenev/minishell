/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stefan <stefan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/04 22:21:08 by stfn              #+#    #+#             */
/*   Updated: 2024/12/08 23:02:38 by stefan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include "minishell.h"

/* 
 * Advance the parser to the next token. If the current token is NULL, EOF, 
 * or an error token, the function does nothing. Otherwise, it moves the 
 * parser's current token pointer to the next token in the token list.
 */
void	parser_advance(t_parser *parser)
{
	if (!parser->current_token || parser->current_token->type == TOKEN_EOF
		|| parser->current_token->type == TOKEN_ERROR)
	{
		return ;
	}
	parser->current_token = parser->current_token->next;
}

/* 
 * Allocate memory for a new AST node and initialize its type to AST_COMMAND. 
 * If memory allocation fails, NULL is returned. Otherwise, the newly created
 * node is returned.
 */
t_ast	*allocate_and_initialize_node(void)
{
	t_ast	*node;

	node = malloc(sizeof(t_ast));
	if (!node)
		return (NULL);
	node->type = AST_COMMAND;
	return (node);
}

/* 
 * Allocate memory for a new command structure and initialize its arguments 
 * and redirections to NULL. If memory allocation fails, NULL is returned. 
 * Otherwise, the newly created command structure is returned.
 */
t_command	*allocate_and_initialize_command(void)
{
	t_command	*cmd;

	cmd = malloc(sizeof(t_command));
	if (!cmd)
		return (NULL);
	cmd->args = NULL;
	cmd->redirections = NULL;
	return (cmd);
}
