/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: spenev <spenev@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/13 13:02:09 by stfn              #+#    #+#             */
/*   Updated: 2024/12/09 09:39:14 by spenev           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ast.h"
#include "minishell.h"

/* 
 * Free the redirection list by iterating through each redirection node,
 * freeing its filename and the redirection node itself.
 */
void	redirection_free(t_redirection *redir)
{
	t_redirection	*tmp;

	while (redir)
	{
		tmp = redir;
		redir = redir->next;
		if (tmp->filename)
			free(tmp->filename);
		free(tmp);
	}
}

/* 
 * Free the command structure by freeing each argument and the redirection list.
 * Finally, free the command itself.
 */
void	command_free(t_command *cmd)
{
	size_t	i;

	if (!cmd)
		return ;
	i = 0;
	if (cmd->args)
	{
		while (cmd->args[i])
		{
			free(cmd->args[i]);
			i++;
		}
		free(cmd->args);
	}
	redirection_free(cmd->redirections);
	free (cmd);
}

/* 
 * Free the abstract syntax tree (AST) recursively. Depending on the AST node
 * type, it frees the appropriate data structures (command, pipeline,
 * logical).
 */
void	ast_free(t_ast *ast)
{
	if (!ast)
		return ;
	if (ast->type == AST_COMMAND)
	{
		if (ast->u_data.command)
			command_free(ast->u_data.command);
	}
	else if (ast->type == AST_PIPELINE)
	{
		ast_free(ast->u_data.pipeline.left);
		ast_free(ast->u_data.pipeline.right);
	}
	else if (ast->type == AST_LOGICAL_AND || ast->type == AST_LOGICAL_OR)
	{
		ast_free(ast->u_data.logical.left);
		ast_free(ast->u_data.logical.right);
	}
	else
		printf("Unknown AST node type: %d\n", ast->type);
	free(ast);
}
