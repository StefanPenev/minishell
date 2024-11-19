/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stfn <stfn@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/13 08:55:59 by stfn              #+#    #+#             */
/*   Updated: 2024/11/19 18:32:01 by stfn             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

void debug_token(t_parser *parser) {
    printf("Debug: Current Token Type=%d, Value='%s'\n",
           parser->current_token->type,
           parser->current_token->value);
}

// Advance the parser to the next token
static void parser_advance(t_parser *parser) {
    // Avoid advancing if the current token is NULL, EOF, or an error token
    if (!parser->current_token || parser->current_token->type == TOKEN_EOF || parser->current_token->type == TOKEN_ERROR) {
        return;  // Do not advance if we are at EOF or there is an error
    }

    debug_token(parser);  // Debug before advancing
    parser->current_token = parser->current_token->next;
    debug_token(parser);  // Debug after advancing
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
// Parse a redirection
t_redirection *parse_redirection(t_parser *parser) {
    t_redirection *redir = malloc(sizeof(t_redirection));
    if (!redir) {
        fprintf(stderr, "Error: Memory allocation failed for redirection.\n");
        return NULL;
    }

    // Check if the token is a valid redirection operator
    if (parser->current_token->type != TOKEN_REDIRECT_OUT &&
        parser->current_token->type != TOKEN_REDIRECT_IN &&
        parser->current_token->type != TOKEN_APPEND &&
        parser->current_token->type != TOKEN_HEREDOC) {
        free(redir);
        return NULL;
    }

    t_redirection_type redir_type;
    if (!map_token_to_redirection(parser->current_token->type, &redir_type)) {
        free(redir);
        return NULL;
    }

    redir->type = redir_type;
    parser_advance(parser); // Move past the redirection operator

    if (redir->type == HEREDOC) {
        // Ensure the next token is the delimiter (WORD)
        if (parser->current_token->type != TOKEN_WORD) {
            fprintf(stderr, "Error: Expected heredoc delimiter, but found '%s'.\n",
                    parser->current_token->value);
            free(redir);
            return NULL;
        }

        // Set the delimiter as the filename
        redir->filename = strdup(parser->current_token->value);
        if (!redir->filename) {
            fprintf(stderr, "Error: Memory allocation failed for delimiter.\n");
            free(redir);
            return NULL;
        }

        parser_advance(parser); // Move past the delimiter

        // Initialize heredoc content buffer
        size_t content_capacity = 1024;
        redir->heredoc_content = malloc(content_capacity);
        if (!redir->heredoc_content) {
            fprintf(stderr, "Error: Memory allocation failed for heredoc content.\n");
            free(redir->filename);
            free(redir);
            return NULL;
        }
        redir->heredoc_content[0] = '\0';

        // Collect heredoc content
        while (parser->current_token) {
            // Check if the token matches the delimiter (end of heredoc)
            if (parser->current_token->type == TOKEN_WORD &&
                strcmp(parser->current_token->value, redir->filename) == 0) {
                parser_advance(parser); // Move past the delimiter
                break; // Exit the loop after encountering the delimiter
            }

            // Append token value to heredoc content
            size_t token_len = strlen(parser->current_token->value);
            if (strlen(redir->heredoc_content) + token_len + 2 > content_capacity) {
                content_capacity *= 2;
                redir->heredoc_content = realloc(redir->heredoc_content, content_capacity);
                if (!redir->heredoc_content) {
                    fprintf(stderr, "Error: Memory reallocation failed for heredoc content.\n");
                    free(redir->filename);
                    free(redir);
                    return NULL;
                }
            }

            strcat(redir->heredoc_content, parser->current_token->value);
            strcat(redir->heredoc_content, "\n"); // Add a newline for heredoc formatting

            parser_advance(parser); // Move to the next token
        }
    } else {
        // Handle other types of redirection (e.g., >, >>, <)
        if (parser->current_token->type == TOKEN_WORD) {
            redir->filename = strdup(parser->current_token->value);
            if (!redir->filename) {
                fprintf(stderr, "Error: Memory allocation failed for filename.\n");
                free(redir);
                return NULL;
            }
            parser_advance(parser); // Move past the filename
        } else {
            fprintf(stderr, "Error: Expected filename, but found '%s'.\n",
                    parser->current_token->value);
            free(redir);
            return NULL;
        }
    }

    return redir;
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

    // Collect arguments (if any)
    size_t args_size = 0;
    size_t args_capacity = 10;
    cmd->args = malloc(sizeof(char *) * args_capacity);
    if (!cmd->args) {
        free(cmd);
        free(node);
        return NULL;
    }

    // Collect all words as arguments for the command
    while (parser->current_token->type == TOKEN_WORD) {
        if (args_size >= args_capacity - 1) {
            // Resize arguments array if necessary
            args_capacity *= 2;
            char **new_args = realloc(cmd->args, sizeof(char *) * args_capacity);
            if (!new_args) {
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

    cmd->args[args_size] = NULL; // Null-terminate the argument array

    // Parse redirections associated with this command
    while (1) {
        t_redirection *redir = parse_redirection(parser);
        if (!redir) {
            break;  // No more redirections, break the loop
        }

        // Add the redirection to the command's redirection list
        redir->next = cmd->redirections;
        cmd->redirections = redir;
    }

    // If no arguments and no redirections were found, the command is malformed
    if (args_size == 0 && !cmd->redirections) {
        free(cmd->args);
        free(cmd);
        free(node);
        return NULL;
    }

    node->u_data.command = cmd;
    return node;
}

// Parse a pipeline
t_ast *parse_pipeline(t_parser *parser) {
    t_ast *left = parse_command(parser);
    if (!left) {
        fprintf(stderr, "Error: Expected command before '|'.\n");
        return NULL;
    }

    while (parser->current_token->type == TOKEN_PIPE) {
        parser_advance(parser);
        t_ast *right = parse_command(parser);
        if (!right) {
            fprintf(stderr, "Error: Expected command after '|'.\n");
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

// Parse a parenthesized expression
t_ast *parse_parenthesized_expression(t_parser *parser) {
    // Check for opening parenthesis
    if (parser->current_token->type != TOKEN_LEFT_PAREN)
        return NULL;

    // Advance past the opening parenthesis
    parser_advance(parser);

    // Parse the logical expression inside the parentheses
    t_ast *expr = parse_logical_expression(parser);
    if (!expr) {
        fprintf(stderr, "Error: Invalid expression inside parentheses.\n");
        return NULL;
    }

    // Check if the current token is a closing parenthesis
    if (parser->current_token->type != TOKEN_RIGHT_PAREN) {
        fprintf(stderr, "Error: Unmatched '('.\n");
        ast_free(expr);  // Free the AST that was partially parsed
        return NULL;
    }

    // Advance past the closing parenthesis
    parser_advance(parser);

    return expr;  // Return the parsed expression
}

// Parse a logical expression (AND, OR)
t_ast *parse_logical_expression(t_parser *parser) {
    t_ast *left = parse_pipeline(parser);
    if (!left)
        return NULL;

    while (parser->current_token->type == TOKEN_AND ||
           parser->current_token->type == TOKEN_OR) {
        t_token_type op = parser->current_token->type;
        parser_advance(parser);

        // Ensure there's a valid right operand
        t_ast *right = parse_pipeline(parser);
        if (!right) {
            fprintf(stderr, "Error: Expected command after '%s'.\n",
                op == TOKEN_AND ? "&&" : "||");
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

// Parse a command line
t_ast *parse_command_line(t_parser *parser) {
    t_ast *expr = NULL;

    // Handle parenthesized expressions or logical expressions
    if (parser->current_token->type == TOKEN_LEFT_PAREN) {
        expr = parse_parenthesized_expression(parser);
        if (!expr) {
            return NULL;
        }
    } else {
        expr = parse_logical_expression(parser);
        if (!expr) {
            return NULL;
        }
    }

    // Handle any trailing redirections (after the main expression)
    while (map_token_to_redirection(parser->current_token->type, NULL)) {
        t_redirection *redir = parse_redirection(parser);
        if (!redir) {
            ast_free(expr);
            return NULL;
        }

        // Attach the redirection to the command at the root of the 
        if (expr->type == AST_COMMAND) {
            redir->next = expr->u_data.command->redirections;
            expr->u_data.command->redirections = redir;
        } else {
            fprintf(stderr, "Error: Redirection not associated with a command.\n");
            ast_free(expr);
            free(redir);
            return NULL;
        }
    }

    return expr;  // Return the parsed expression
}

// Main parse function
t_ast *parse_tokens(t_token *tokens) {
    t_parser parser;
    parser.current_token = tokens;

    // Call the main parsing function (parse_command_line)
    t_ast *ast = parse_command_line(&parser);

    // **Ensure the parser has reached the end of input (EOF)**
    if (parser.current_token->type != TOKEN_EOF) {
        // If we haven't reached EOF, report the unexpected token
        fprintf(stderr, "Error: Unexpected token '%s' at end of input.\n",
                parser.current_token->value);

        // Clean up and free the parsed AST in case of error
        ast_free(ast);  // Free the partially parsed AST
        return NULL;    // Return NULL to indicate an error
    }

    // Return the successfully parsed AST
    return ast;
}
