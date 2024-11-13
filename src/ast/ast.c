/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stfn <stfn@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/13 13:02:09 by stfn              #+#    #+#             */
/*   Updated: 2024/11/13 13:27:37 by stfn             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ast.h"

// Free redirections
static void redirection_free(t_redirection *redir) {
    while (redir) {
        t_redirection *tmp = redir;
        redir = redir->next;
        if (tmp->filename)
            free(tmp->filename);
        free(tmp);
    }
}

// Free commands
static void command_free(t_command *cmd) {
    if (!cmd)
        return;
    if (cmd->args) {
        for (size_t i = 0; cmd->args[i]; i++)
            free(cmd->args[i]);
        free(cmd->args);
    }
    redirection_free(cmd->redirections);
    free(cmd);
}

// Recursive AST free
void ast_free(t_ast *ast) {
    if (!ast)
        return;
    switch (ast->type) {
        case AST_COMMAND:
            command_free(ast->u_data.command);
            break;
        case AST_PIPELINE:
            ast_free(ast->u_data.pipeline.left);
            ast_free(ast->u_data.pipeline.right);
            break;
        case AST_LOGICAL_AND:
        case AST_LOGICAL_OR:
            ast_free(ast->u_data.logical.left);
            ast_free(ast->u_data.logical.right);
            break;
        // Handle other node types if added
        default:
            break;
    }
    free(ast);
}
