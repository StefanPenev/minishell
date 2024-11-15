/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stfn <stfn@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/09 21:41:55 by stfn              #+#    #+#             */
/*   Updated: 2024/11/16 00:30:21 by stfn             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "signals.h"
#include "lexer.h"
#include "parser.h"
#include "ast.h"
#include "minishell.h"


//////////////////////////
#include <stdlib.h>

// Print function prototypes
void print_redirection(t_redirection *redir, int indent);
void print_command(t_command *cmd, int indent);
void print_ast(t_ast *ast, int indent);

// Helper to print indentation
void print_indent(int indent) {
    for (int i = 0; i < indent; i++)
        printf("  ");  // Two spaces per indentation level
}

// Print a single redirection
void print_redirection(t_redirection *redir, int indent) {
    while (redir) {
        print_indent(indent);
        switch (redir->type) {
            case REDIRECT_INPUT:
                printf("Redirection: Input from %s\n", redir->filename);
                break;
            case REDIRECT_OUTPUT:
                printf("Redirection: Output to %s\n", redir->filename);
                break;
            case REDIRECT_APPEND:
                printf("Redirection: Append to %s\n", redir->filename);
                break;
            case HEREDOC:
                printf("Redirection: Heredoc %s\n", redir->filename);
                break;
            default:
                printf("Redirection: Unknown type\n");
                break;
        }
        redir = redir->next;
    }
}

// Print a command node
void print_command(t_command *cmd, int indent) {
    print_indent(indent);
    printf("Command:\n");

    // Print arguments
    print_indent(indent + 1);
    printf("Arguments: ");
    for (size_t i = 0; cmd->args && cmd->args[i]; i++) {
        printf("%s ", cmd->args[i]);
    }
    printf("\n");

    // Print redirections
    if (cmd->redirections) {
        print_indent(indent + 1);
        printf("Redirections:\n");
        print_redirection(cmd->redirections, indent + 2);
    }
}

// Recursive function to print AST
void print_ast(t_ast *ast, int indent) {
    if (!ast) return;

    switch (ast->type) {
        case AST_COMMAND:
            print_command(ast->u_data.command, indent);
            break;

        case AST_PIPELINE:
            print_indent(indent);
            printf("Pipeline:\n");
            print_indent(indent + 1);
            printf("Left:\n");
            print_ast(ast->u_data.pipeline.left, indent + 2);
            print_indent(indent + 1);
            printf("Right:\n");
            print_ast(ast->u_data.pipeline.right, indent + 2);
            break;

        case AST_LOGICAL_AND:
            print_indent(indent);
            printf("Logical AND:\n");
            print_indent(indent + 1);
            printf("Left:\n");
            print_ast(ast->u_data.logical.left, indent + 2);
            print_indent(indent + 1);
            printf("Right:\n");
            print_ast(ast->u_data.logical.right, indent + 2);
            break;

        case AST_LOGICAL_OR:
            print_indent(indent);
            printf("Logical OR:\n");
            print_indent(indent + 1);
            printf("Left:\n");
            print_ast(ast->u_data.logical.left, indent + 2);
            print_indent(indent + 1);
            printf("Right:\n");
            print_ast(ast->u_data.logical.right, indent + 2);
            break;

        // Add additional cases here for other AST node types as needed

        default:
            print_indent(indent);
            printf("Unknown AST node type\n");
            break;
    }
}
//////////////////////////

int	main(int argc, char **argv, char **envp)
{
	char			*input;
	t_token			*tokens;
	t_term_context	ctx;
	//t_ast			*ast;

	(void)argc;
	(void)argv;
	handle_signals();
	disable_echoctl(&ctx);
	while (1)
	{
		input = readline("minishell$ ");
		if (!input)
		{
			printf("exit\n");
			break ;
		}
		if (*input)
			add_history(input);
		tokens = lexer_tokenize(input, envp);
		if (!tokens)
		{
			free(input);
			continue ;
		}
		if (tokens->type == TOKEN_ERROR)
		{
			fprintf(stderr, "Error: %s\n", lexer_get_error_message(tokens));
			lexer_free_tokens(tokens);
			free(input);
			continue ;
		}
		// ast = parse_tokens(tokens);
		// if (!ast)
		// {
		// 	fprintf(stderr, "Parsing error\n");
		// 	lexer_free_tokens(tokens);
		// 	free(input);
		// 	continue ;
		// }
        // print_ast(ast, 0);
        
		//process

        print_tokens(tokens);

		lexer_free_tokens(tokens);
		free(input);
	}
	restore_terminal(&ctx);
	return (0);
}
