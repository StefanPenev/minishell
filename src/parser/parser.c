/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stfn <stfn@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/13 08:55:59 by stfn              #+#    #+#             */
/*   Updated: 2024/11/23 22:43:32 by stfn             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

// void debug_token(t_parser *parser) {
//     printf("Debug: Current Token Type=%d, Value='%s'\n",
//            parser->current_token->type,
//            parser->current_token->value);
// }

// Advance the parser to the next token
static void parser_advance(t_parser *parser) {
    // Avoid advancing if the current token is NULL, EOF, or an error token
    if (!parser->current_token || parser->current_token->type == TOKEN_EOF || parser->current_token->type == TOKEN_ERROR) {
        return;  // Do not advance if we are at EOF or there is an error
    }

    //debug_token(parser);  // Debug before advancing
    parser->current_token = parser->current_token->next;
    //debug_token(parser);  // Debug after advancing
}

// Forward declarations
t_ast *parse_command(t_parser *parser);
t_ast *parse_pipeline(t_parser *parser);
t_ast *parse_parenthesized_expression(t_parser *parser);
t_ast *parse_logical_expression(t_parser *parser);
static int map_token_to_redirection(t_token_type token_type, t_redirection_type *redir_type);

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

void add_argument_to_command(t_command *command, const char *arg) {
    // Ensure the command's args array is big enough to hold the new argument
    size_t args_count = 0;
    while (command->args[args_count] != NULL) {
        args_count++;
    }

    // Resize the argument list if necessary
    if (args_count >= command->args_capacity - 1) {
        size_t new_capacity = command->args_capacity * 2;
        command->args = realloc(command->args, new_capacity * sizeof(char *));
        if (!command->args) {
            fprintf(stderr, "Failed to allocate memory for arguments\n");
            exit(EXIT_FAILURE);  // Or handle error properly
        }
        command->args_capacity = new_capacity;
    }

    // Add the new argument
    command->args[args_count] = strdup(arg);
    command->args[args_count + 1] = NULL;  // Null-terminate the list
}

void parse_wildcard(t_token *token, t_command *command) {
    // Wildcard handling code here
    if (token->type == TOKEN_WILDCARD) {
        char **expanded_files = expand_wildcard();
        if (!expanded_files) {
            fprintf(stderr, "Wildcard expansion failed\n");
            return;
        }

        // Add expanded filenames to the command's argument list
        for (int i = 0; expanded_files[i] != NULL; i++) {
            add_argument_to_command(command, expanded_files[i]);
            free(expanded_files[i]); // Free each expanded filename
        }
        free(expanded_files); // Free the array itself
    }
}

// Helper function to check if a token is a redirection token
static int is_redirection_token(t_token_type token_type) {
    return token_type == TOKEN_REDIRECT_OUT ||
           token_type == TOKEN_REDIRECT_IN ||
           token_type == TOKEN_APPEND ||
           token_type == TOKEN_HEREDOC;
}

// Parse a redirection
t_redirection *parse_redirection(t_parser *parser) {
    // Check if the current token exists and is a redirection operator
    if (!parser->current_token ||
        !is_redirection_token(parser->current_token->type)) {
        return NULL;
    }

    t_redirection *redir = malloc(sizeof(t_redirection));
    if (!redir) {
        fprintf(stderr, "Error: Memory allocation failed for redirection.\n");
        return NULL;
    }

    // Initialize the redirection struct
    redir->type = 0;
    redir->filename = NULL;
    redir->heredoc_content = NULL;
    redir->next = NULL;

    // Map token type to redirection type
    map_token_to_redirection(parser->current_token->type, &redir->type);

    parser_advance(parser); // Move past the redirection operator

    if (redir->type == HEREDOC) {
        // Expect the next token to be the heredoc delimiter (WORD)
        if (!parser->current_token ||
            parser->current_token->type != TOKEN_WORD ||
            !parser->current_token->value) {
            fprintf(stderr, "Error: Expected heredoc delimiter after '<<', but found '%s'.\n",
                    parser->current_token && parser->current_token->value ? parser->current_token->value : "NULL");
            free(redir);
            return NULL;
        }

        // Store the heredoc delimiter
        redir->filename = strdup(parser->current_token->value);
        if (!redir->filename) {
            fprintf(stderr, "Error: Memory allocation failed for heredoc delimiter.\n");
            free(redir);
            return NULL;
        }

        parser_advance(parser); // Move past the delimiter token

        // Initialize heredoc content buffer
        size_t content_capacity = 1024;
        size_t content_length = 0;
        redir->heredoc_content = malloc(content_capacity);
        if (!redir->heredoc_content) {
            fprintf(stderr, "Error: Memory allocation failed for heredoc content.\n");
            free(redir->filename);
            free(redir);
            return NULL;
        }
        redir->heredoc_content[0] = '\0';

        // Collect heredoc content until the delimiter is found
        int delimiter_found = 0;
        while (parser->current_token && parser->current_token->type != TOKEN_EOF) {
            // Check if the token matches the delimiter
            if (parser->current_token->type == TOKEN_WORD &&
                parser->current_token->value &&
                strcmp(parser->current_token->value, redir->filename) == 0) {
                delimiter_found = 1;
                parser_advance(parser); // Move past the delimiter token
                break;
            }

            // Ensure parser->current_token->value is not NULL
            if (!parser->current_token->value) {
                fprintf(stderr, "Error: Token value is NULL in heredoc content.\n");
                free(redir->filename);
                free(redir->heredoc_content);
                free(redir);
                return NULL;
            }

            // Ensure buffer has enough space for new content
            size_t token_len = strlen(parser->current_token->value) + 1;
            if (content_length + token_len + 1 > content_capacity) {
                content_capacity *= 2;
                char *new_content = realloc(redir->heredoc_content, content_capacity);
                if (!new_content) {
                    fprintf(stderr, "Error: Memory reallocation failed for heredoc content.\n");
                    free(redir->filename);
                    free(redir->heredoc_content);
                    free(redir);
                    return NULL;
                }
                redir->heredoc_content = new_content;
            }

            // Append token value to heredoc content
            strcat(redir->heredoc_content, parser->current_token->value);
            strcat(redir->heredoc_content, "\n");
            content_length += token_len;

            parser_advance(parser); // Move to the next token
        }

        // Check if the delimiter was found
        if (!delimiter_found) {
            fprintf(stderr, "Error: Expected heredoc delimiter '%s' not found before end of input.\n", redir->filename);
            free(redir->filename);
            free(redir->heredoc_content);
            free(redir);
            return NULL;
        }
    } else {
        // Handle standard redirection types (>, >>, <)
        if (!parser->current_token ||
            parser->current_token->type != TOKEN_WORD ||
            !parser->current_token->value) {
            fprintf(stderr, "Error: Expected filename after redirection operator, but found '%s'.\n",
                    parser->current_token && parser->current_token->value ? parser->current_token->value : "NULL");
            free(redir);
            return NULL;
        }

        redir->filename = strdup(parser->current_token->value);
        if (!redir->filename) {
            fprintf(stderr, "Error: Memory allocation failed for filename.\n");
            free(redir);
            return NULL;
        }
        parser_advance(parser); // Move past the filename token
    }

    return redir;
}

void free_redirections(t_redirection *redir) {
    while (redir) {
        t_redirection *next = redir->next;

        // Safeguard in case filename is NULL
        if (redir->filename) {
            free(redir->filename);  // Free the dynamically allocated filename
        }

        free(redir);  // Free the redirection node itself
        redir = next; // Move to the next redirection
    }
}

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

    // Initialize arguments array
    size_t args_size = 0;
    size_t args_capacity = 10;
    cmd->args = malloc(sizeof(char *) * args_capacity);
    if (!cmd->args) {
        free(cmd);
        free(node);
        return NULL;
    }

    // Collect all words as arguments
    while (parser->current_token && parser->current_token->type == TOKEN_WORD) {
        if (args_size >= args_capacity - 1) {
            // Resize arguments array
            args_capacity *= 2;
            char **new_args = realloc(cmd->args, sizeof(char *) * args_capacity);
            if (!new_args) {
                for (size_t i = 0; i < args_size; i++) {
                    free(cmd->args[i]);
                }
                free(cmd->args);
                free(cmd);
                free(node);
                return NULL;
            }
            cmd->args = new_args;
        }
        cmd->args[args_size++] = strdup(parser->current_token->value);
        if (!cmd->args[args_size - 1]) { // Handle strdup failure
            for (size_t i = 0; i < args_size - 1; i++) {
                free(cmd->args[i]);
            }
            free(cmd->args);
            free(cmd);
            free(node);
            return NULL;
        }
        parser_advance(parser);
    }

    cmd->args[args_size] = NULL; // Null-terminate the argument array

    // Handle wildcard expansion if we encounter a wildcard token
    if (parser->current_token && parser->current_token->type == TOKEN_WILDCARD) {
        parse_wildcard(parser->current_token, cmd); // Assume parse_wildcard cleans up internally
        parser_advance(parser);  // Move past the wildcard token
    }

    // Parse redirections associated with this command
    while (parser->current_token && is_redirection_token(parser->current_token->type)) {
        t_redirection *redir = parse_redirection(parser);
        if (!redir) {
            for (size_t i = 0; i < args_size; i++) {
                free(cmd->args[i]);
            }
            free(cmd->args);
            free_redirections(cmd->redirections); // Helper function to free redirections
            free(cmd);
            free(node);
            return NULL;
        }

        // Add the redirection to the command's redirection list
        redir->next = cmd->redirections;
        cmd->redirections = redir;
    }

    // If no arguments and no redirections were found, the command is malformed
    if (args_size == 0 && !cmd->redirections) {
        for (size_t i = 0; i < args_size; i++) {
            free(cmd->args[i]);
        }
        free(cmd->args);
        free_redirections(cmd->redirections); // Free redirections if any
        free(cmd);
        free(node);
        return NULL;
    }

    node->u_data.command = cmd;
    return node;
}

t_ast *parse_pipeline(t_parser *parser) {
    t_ast *left = parse_command(parser);
    if (!left)
        return NULL;

    while (parser->current_token && parser->current_token->type == TOKEN_PIPE) {
        parser_advance(parser);
        t_ast *right = parse_command(parser);
        if (!right) {
            fprintf(stderr, "Error: Expected command after '|'.\n");
            ast_free(left);  // Clean up left node
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

        left = pipe_node;  // Keep expanding the pipeline, `left` becomes the root
    }

    return left;
}

// Parse a parenthesized expression
t_ast *parse_parenthesized_expression(t_parser *parser) {
    if (!parser->current_token || parser->current_token->type != TOKEN_LEFT_PAREN)
        return NULL;

    parser_advance(parser); // Move past '('
    t_ast *expr = parse_logical_expression(parser);
    if (!expr) {
        fprintf(stderr, "Error: Invalid expression inside parentheses.\n");
        return NULL;
    }

    if (!parser->current_token || parser->current_token->type != TOKEN_RIGHT_PAREN) {
        fprintf(stderr, "Error: Expected ')' after expression.\n");
        ast_free(expr);
        return NULL;
    }

    parser_advance(parser); // Move past ')'
    return expr;
}

t_ast *parse_logical_expression(t_parser *parser) {
    t_ast *left = NULL;

    if (parser->current_token && parser->current_token->type == TOKEN_LEFT_PAREN) {
        left = parse_parenthesized_expression(parser);
    } else {
        left = parse_pipeline(parser);
    }

    if (!left)
        return NULL;

    while (parser->current_token &&
           (parser->current_token->type == TOKEN_AND ||
            parser->current_token->type == TOKEN_OR)) {
        t_ast_node_type type = (parser->current_token->type == TOKEN_AND) ? AST_LOGICAL_AND : AST_LOGICAL_OR;
        parser_advance(parser);

        t_ast *right = NULL;
        if (parser->current_token && parser->current_token->type == TOKEN_LEFT_PAREN) {
            right = parse_parenthesized_expression(parser);
        } else {
            right = parse_pipeline(parser);
        }

        if (!right) {
            fprintf(stderr, "Error: Expected pipeline after logical operator.\n");
            ast_free(left);
            return NULL;
        }

        t_ast *node = malloc(sizeof(t_ast));
        if (!node) {
            ast_free(left);
            ast_free(right);
            return NULL;
        }
        node->type = type;
        node->u_data.logical.left = left;
        node->u_data.logical.right = right;

        left = node;
    }

    return left;
}

// Parse a command line
t_ast *parse_command_line(t_parser *parser) {
    t_ast *expr = parse_logical_expression(parser);
    if (!expr)
        return NULL;

    // Handle any trailing redirections (after the main expression)
    while (parser->current_token && is_redirection_token(parser->current_token->type)) {
        t_redirection *redir = parse_redirection(parser);
        if (!redir) {
            ast_free(expr);
            return NULL;
        }

        // Attach the redirection to the command at the root of the expression
        if (expr->type == AST_COMMAND) {
            redir->next = expr->u_data.command->redirections;
            expr->u_data.command->redirections = redir;
        } else {
            fprintf(stderr, "Error: Redirection not associated with a command.\n");
            ast_free(expr);
            free_redirections(redir);
            return NULL;
        }
    }

    return expr;
}

// Main parse function
t_ast *parse_tokens(t_token *tokens) {
    t_parser parser;
    parser.current_token = tokens;

    t_ast *ast = parse_command_line(&parser);

    // Ensure the parser has reached the end of input (EOF)
    if (!parser.current_token || parser.current_token->type != TOKEN_EOF) {
        fprintf(stderr, "Error: Unexpected %s at end of input.\n",
                parser.current_token && parser.current_token->value ? parser.current_token->value : "end of input");

        ast_free(ast);
        return NULL;
    }

    return ast;
}