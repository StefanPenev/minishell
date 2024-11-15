/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stfn <stfn@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/13 08:55:59 by stfn              #+#    #+#             */
/*   Updated: 2024/11/15 13:54:33 by stfn             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>


/*<command_line> ::= <logical_expression> TOKEN_EOF

<logical_expression> ::= <pipeline>
                       | <logical_expression> TOKEN_AND <pipeline>
                       | <logical_expression> TOKEN_OR <pipeline>

<pipeline> ::= <command>
             | <pipeline> TOKEN_PIPE <command>

<command> ::= TOKEN_WORD <arguments> <redirections>

<arguments> ::= TOKEN_WORD <arguments>
              | ε

<redirections> ::= <redirection> <redirections>
                 | ε

<redirection> ::= TOKEN_REDIRECT_IN TOKEN_WORD
                | TOKEN_REDIRECT_OUT TOKEN_WORD
                | TOKEN_APPEND TOKEN_WORD
                | TOKEN_HEREDOC TOKEN_WORD

<parenthesized_expression> ::= TOKEN_LEFT_PAREN <logical_expression> TOKEN_RIGHT_PAREN*/

// <command_line> ::= <logical_expression> TOKEN_EOF

// <logical_expression> ::= <pipeline>
//                        | <logical_expression> TOKEN_AND <pipeline>
//                        | <logical_expression> TOKEN_OR <pipeline>
//                        | <parenthesized_expression>

// <parenthesized_expression> ::= TOKEN_LEFT_PAREN <logical_expression> TOKEN_RIGHT_PAREN

// <pipeline> ::= <command>
//              | <pipeline> TOKEN_PIPE <command>

// <command> ::= <builtin_command> <redirections>
//             | <external_command> <redirections>

// <builtin_command> ::= TOKEN_ECHO <echo_options> <arguments>
//                     | TOKEN_CD <path>
//                     | TOKEN_PWD
//                     | TOKEN_EXPORT <export_arguments>
//                     | TOKEN_UNSET <variable>
//                     | TOKEN_ENV
//                     | TOKEN_EXIT <exit_status>

// <echo_options> ::= TOKEN_FLAG_N
//                  | ε

// <external_command> ::= TOKEN_WORD <arguments>

// <arguments> ::= TOKEN_WORD <arguments>
//               | TOKEN_QUOTED_STRING <arguments>
//               | TOKEN_ENV_VAR <arguments>
//               | ε

// <export_arguments> ::= <variable_assignment> <export_arguments>
//                      | ε

// <variable_assignment> ::= TOKEN_WORD TOKEN_EQUALS TOKEN_WORD

// <redirections> ::= <redirection> <redirections>
//                  | ε

// <redirection> ::= TOKEN_REDIRECT_IN TOKEN_WORD
//                 | TOKEN_REDIRECT_OUT TOKEN_WORD
//                 | TOKEN_APPEND TOKEN_WORD
//                 | TOKEN_HEREDOC TOKEN_WORD

// <path> ::= TOKEN_WORD

// <variable> ::= TOKEN_WORD

// <exit_status> ::= TOKEN_NUMBER
//                 | ε

// Mapping function from t_token_type to t_redirection_type
static int map_token_to_redirection(t_token_type token_type, t_redirection_type *redir_type) {
    if (!redir_type)
        return 0; // Failure due to null pointer

    switch (token_type) {
        case TOKEN_REDIRECT_IN:
            *redir_type = REDIRECT_INPUT;
            return 1;
        case TOKEN_REDIRECT_OUT:
            *redir_type = REDIRECT_OUTPUT;
            return 1;
        case TOKEN_APPEND:
            *redir_type = REDIRECT_APPEND;
            return 1;
        case TOKEN_HEREDOC:
            *redir_type = HEREDOC;
            return 1;
        default:
            return 0; // Unknown redirection type
    }
}

// Utility function to advance the parser to the next token
static void parser_advance(t_parser *parser) {
    if (parser->current_token->type != TOKEN_EOF)
        parser->current_token = parser->current_token->next;
}

// Forward declarations
t_ast *parse_logical_expression(t_parser *parser);

// Parse a simple command
t_ast *parse_command(t_parser *parser) {
    t_ast *node = malloc(sizeof(t_ast));
    if (!node)
        return NULL;
    node->type = AST_COMMAND;

    t_command *cmd = malloc(sizeof(t_command));
    if (!cmd) {
        free(node);
        return NULL;
    }
    cmd->args = NULL;
    cmd->redirections = NULL;

    // Collect arguments
    size_t args_size = 0;
    size_t args_capacity = 10;
    cmd->args = malloc(sizeof(char*) * args_capacity);
    if (!cmd->args) {
        free(cmd);
        free(node);
        return NULL;
    }

    while (parser->current_token->type == TOKEN_WORD) {
        if (args_size >= args_capacity - 1) {
            args_capacity *= 2;
            char **new_args = realloc(cmd->args, sizeof(char*) * args_capacity);
            if (!new_args) {
                // Free allocated memory
                for (size_t i = 0; i < args_size; i++)
                    free(cmd->args[i]);
                free(cmd->args);
                free(cmd);
                free(node);
                return NULL;
            }
            cmd->args = new_args;
        }
        cmd->args[args_size++] = strdup(parser->current_token->value);
        parser_advance(parser);
    }
    cmd->args[args_size] = NULL;

    // Handle redirections
    while (parser->current_token->type == TOKEN_REDIRECT_IN ||
           parser->current_token->type == TOKEN_REDIRECT_OUT ||
           parser->current_token->type == TOKEN_APPEND ||
           parser->current_token->type == TOKEN_HEREDOC) {
        t_redirection *redir = malloc(sizeof(t_redirection));
        if (!redir) {
            // Free allocated memory
            for (size_t i = 0; i < args_size; i++)
                free(cmd->args[i]);
            free(cmd->args);
            free(cmd);
            free(node);
            return NULL;
        }

        // Map token type to redirection type
        if (!map_token_to_redirection(parser->current_token->type, &redir->type)) {
            fprintf(stderr, "Error: Unknown redirection type.\n");
            free(redir);
            // Free allocated memory
            for (size_t i = 0; i < args_size; i++)
                free(cmd->args[i]);
            free(cmd->args);
            free(cmd);
            free(node);
            return NULL;
        }

        parser_advance(parser);
        if (parser->current_token->type != TOKEN_WORD) {
            fprintf(stderr, "Error: Expected filename after redirection.\n");
            free(redir);
            // Free allocated memory
            for (size_t i = 0; i < args_size; i++)
                free(cmd->args[i]);
            free(cmd->args);
            free(cmd);
            free(node);
            return NULL;
        }
        redir->filename = strdup(parser->current_token->value);
        parser_advance(parser);
        redir->next = cmd->redirections;
        cmd->redirections = redir;
    }

    node->u_data.command = cmd;
    return node;
}

// Parse a pipeline of commands
t_ast *parse_pipeline(t_parser *parser) {
    t_ast *left = parse_command(parser);
    if (!left)
        return NULL;

    while (parser->current_token->type == TOKEN_PIPE) {
        parser_advance(parser);
        t_ast *right = parse_command(parser);
        if (!right) {
            // Handle error: expected command after pipe
            ast_free(left);
            return NULL;
        }
        t_ast *pipe_node = malloc(sizeof(t_ast));
        if (!pipe_node) {
            ast_free(left);
            ast_free(right);
            return NULL;
        }
        pipe_node->type = AST_PIPELINE;
        pipe_node->u_data.pipeline.left = left;
        pipe_node->u_data.pipeline.right = right;
        left = pipe_node;
    }

    return left;
}

// Parse logical expressions (&&, ||)
t_ast *parse_logical_expression(t_parser *parser) {
    t_ast *left = parse_pipeline(parser);
    if (!left)
        return NULL;

    while (parser->current_token->type == TOKEN_AND ||
           parser->current_token->type == TOKEN_OR) {
        t_token_type op = parser->current_token->type;
        parser_advance(parser);
        t_ast *right = parse_pipeline(parser);
        if (!right) {
            // Handle error: expected command after logical operator
            ast_free(left);
            return NULL;
        }
        t_ast *logic_node = malloc(sizeof(t_ast));
        if (!logic_node) {
            ast_free(left);
            ast_free(right);
            return NULL;
        }
        logic_node->type = (op == TOKEN_AND) ? AST_LOGICAL_AND : AST_LOGICAL_OR;
        logic_node->u_data.logical.left = left;
        logic_node->u_data.logical.right = right;
        left = logic_node;
    }

    return left;
}

// Parse expressions within parentheses
t_ast *parse_parentheses(t_parser *parser) {
    if (parser->current_token->type != TOKEN_LEFT_PAREN)
        return NULL;
    parser_advance(parser);
    t_ast *expr = parse_logical_expression(parser);
    if (!expr)
        return NULL;
    if (parser->current_token->type != TOKEN_RIGHT_PAREN) {
        ast_free(expr);
        return NULL;
    }
    parser_advance(parser);
    return expr;
}

// Parse the entire command line
t_ast *parse_command_line(t_parser *parser) {
    return parse_logical_expression(parser);
}

// Main parse function
t_ast *parse_tokens(t_token *tokens) {
    t_parser parser;
    parser_init(&parser, tokens);

    t_ast *ast = parse_command_line(&parser);

    if (!ast)
        return NULL;

    if (parser.current_token->type != TOKEN_EOF) {
        // Handle error: unexpected token
        ast_free(ast);
        return NULL;
    }

    return ast;
}

void parser_init(t_parser *parser, t_token *tokens) {
    parser->current_token = tokens;
}
