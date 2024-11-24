/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stfn <stfn@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/13 13:02:09 by stfn              #+#    #+#             */
/*   Updated: 2024/11/24 18:26:11 by stfn             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ast.h"
#include "minishell.h"

// Free redirections
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

// Free commands
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

// AST free
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
