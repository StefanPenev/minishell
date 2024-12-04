/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stfn <stfn@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/04 22:21:08 by stfn              #+#    #+#             */
/*   Updated: 2024/12/04 22:26:50 by stfn             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include "minishell.h"

// Advance the parser to the next token
void	parser_advance(t_parser *parser)
{
	if (!parser->current_token || parser->current_token->type == TOKEN_EOF
		|| parser->current_token->type == TOKEN_ERROR)
	{
		return ;
	}
	parser->current_token = parser->current_token->next;
}

t_ast	*allocate_and_initialize_node(void)
{
	t_ast	*node;

	node = malloc(sizeof(t_ast));
	if (!node)
		return (NULL);
	node->type = AST_COMMAND;
	return (node);
}

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
