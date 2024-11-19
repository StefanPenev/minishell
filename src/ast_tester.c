/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_tester.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stfn <stfn@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/18 10:47:54 by stfn              #+#    #+#             */
/*   Updated: 2024/11/19 13:03:37 by stfn             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ast.h"
#include "minishell.h"

// Print function prototypes
void	print_redirection(t_redirection *redir, int indent);
void	print_command(t_command *cmd, int indent);
void	print_ast(t_ast *ast, int indent);

// Helper to print indentation
void	print_indent(int indent)
{
	int	i;

	i = 0;
	while (i < indent)
	{
		printf("  ");
		i++;
	}
}

// Print a single redirection
void	print_redirection(t_redirection *redir, int indent)
{
	while (redir)
	{
		print_indent(indent);
		if (redir->type == REDIRECT_INPUT)
			printf("Redirection: Input from %s\n", redir->filename);
		else if (redir->type == REDIRECT_OUTPUT)
			printf("Redirection: Output to %s\n", redir->filename);
		else if (redir->type == REDIRECT_APPEND)
			printf("Redirection: Append to %s\n", redir->filename);
		else if (redir->type == HEREDOC)
			printf("Redirection: Heredoc %s\n", redir->filename);
		else
			printf("Redirection: Unknown type\n");
		redir = redir->next;
	}
}

// Print a command node
void	print_command(t_command *cmd, int indent)
{
	size_t	i;

	print_indent(indent);
	printf("Command:\n");
	print_indent(indent + 1);
	printf("Arguments: ");
	i = 0;
	while (cmd->args && cmd->args[i])
	{
		printf("%s ", cmd->args[i]);
		i++;
	}
	printf("\n");
	if (cmd->redirections)
	{
		print_indent(indent + 1);
		printf("Redirections:\n");
		print_redirection(cmd->redirections, indent + 2);
	}
}
// void print_command(t_command *cmd, int indent)
// {
//     size_t i;

//     print_indent(indent);
//     printf("Command:\n");
//     // Loop through and print each argument on a new line
//     for (i = 0; cmd->args && cmd->args[i]; i++) {
//         print_indent(indent + 1);
//         printf("Arguments: %s\n", cmd->args[i]);
//     }

//     // If the command has redirections, print them
//     if (cmd->redirections) {
//         print_indent(indent + 1);
//         printf("Redirections:\n");
//         print_redirection(cmd->redirections, indent + 2);
//     }
// }



// Recursive function to print AST
void	print_ast(t_ast *ast, int indent)
{
	if (!ast)
		return ;
	if (ast->type == AST_COMMAND)
	{
		print_command(ast->u_data.command, indent);
	}
	else if (ast->type == AST_PIPELINE)
	{
		print_indent(indent);
		printf("Pipeline:\n");
		print_indent(indent + 1);
		printf("Left:\n");
		print_ast(ast->u_data.pipeline.left, indent + 2);
		print_indent(indent + 1);
		printf("Right:\n");
		print_ast(ast->u_data.pipeline.right, indent + 2);
	}
	else if (ast->type == AST_LOGICAL_AND)
	{
		print_indent(indent);
		printf("Logical AND:\n");
		print_indent(indent + 1);
		printf("Left:\n");
		print_ast(ast->u_data.logical.left, indent + 2);
		print_indent(indent + 1);
		printf("Right:\n");
		print_ast(ast->u_data.logical.right, indent + 2);
	}
	else if (ast->type == AST_LOGICAL_OR)
	{
		print_indent(indent);
		printf("Logical OR:\n");
		print_indent(indent + 1);
		printf("Left:\n");
		print_ast(ast->u_data.logical.left, indent + 2);
		print_indent(indent + 1);
		printf("Right:\n");
		print_ast(ast->u_data.logical.right, indent + 2);
	}
	else
	{
		print_indent(indent);
		printf("Unknown AST node type\n");
	}
}
