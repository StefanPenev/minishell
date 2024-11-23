/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stfn <stfn@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/13 13:02:09 by stfn              #+#    #+#             */
/*   Updated: 2024/11/23 22:41:41 by stfn             ###   ########.fr       */
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
static void	command_free(t_command *cmd)
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

// Recursive AST free
void ast_free(t_ast *ast)
{
    if (!ast)
        return;

    // Debug log for tracking AST node being freed
    printf("Freeing AST node: %p, type: %d\n", ast, ast->type);

    switch (ast->type)
    {
        case AST_COMMAND:
            if (ast->u_data.command)
            {
                printf("Freeing command associated with AST node: %p\n", ast->u_data.command);
                command_free(ast->u_data.command);
            }
            break;

        case AST_PIPELINE:
            if (ast->u_data.pipeline.left || ast->u_data.pipeline.right)
                printf("Freeing pipeline nodes of AST\n");

            // Recursively free left and right pipeline nodes
            ast_free(ast->u_data.pipeline.left);
            ast_free(ast->u_data.pipeline.right);
            break;

        case AST_LOGICAL_AND:
        case AST_LOGICAL_OR:
            if (ast->u_data.logical.left || ast->u_data.logical.right)
                printf("Freeing logical nodes of AST\n");

            // Recursively free left and right logical nodes
            ast_free(ast->u_data.logical.left);
            ast_free(ast->u_data.logical.right);
            break;

        default:
            printf("Unknown AST node type: %d\n", ast->type);
            break;
    }

    // Finally, free the AST node itself
    free(ast);
}