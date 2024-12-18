/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stfn <stfn@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 23:27:57 by stfn              #+#    #+#             */
/*   Updated: 2024/12/07 16:36:45 by stfn             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_H
# define PARSER_H

# include "lexer.h"
# include "ast.h"

typedef struct s_parser
{
	t_token	*current_token;
}	t_parser;

/* ************************************************************************** */
/*                            Parse expressions                               */
/* ************************************************************************** */

t_ast			*parse_tokens(t_token *tokens);
t_ast			*parse_pipeline(t_parser *parser);
t_ast			*parse_command_line(t_parser *parser);
t_ast			*parse_logical_expression(t_parser *parser);

//parse_expression_utils.c
t_ast			*handle_parse_error(t_ast *left);
t_ast_node_type	get_logical_type(t_parser *parser);
t_ast			*handle_allocation_error(t_ast *left, t_ast *right);

t_ast			*parse_parenthesized_expression(t_parser *parser);

/* ************************************************************************** */
/*          				Parser heredoc handling                           */
/* ************************************************************************** */

t_redirection	*allocate_redirection(void);
void			free_redirections(t_redirection *redir);
int				handle_heredoc(t_parser *parser, t_redirection *redir);
int				validate_heredoc_token(t_parser *parser, t_redirection *redir);

/* ************************************************************************** */
/*              			Parser command handling                           */
/* ************************************************************************** */

t_ast			*parse_command(t_parser *parser);
void			parser_advance(t_parser *parser);

/* ************************************************************************** */
/*             			   Parser argument handling                           */
/* ************************************************************************** */

void			handle_wildcards(t_parser *parser, t_command *cmd);
int				collect_arguments(t_parser *parser, t_command *cmd,
					size_t *args_size, size_t *args_capacity);
void			parse_wildcard(t_token *token, t_command *command);
int				initialize_arguments(t_command *cmd, size_t *args_capacity);
void			add_argument_to_command(t_command *command, const char *arg);

/* ************************************************************************** */
/*              		 Parser redirection handling                          */
/* ************************************************************************** */

t_redirection	*parse_redirection(t_parser *parser);
int				is_redirection_token(t_token_type token_type);
int				map_token_to_redirection(t_token_type token_type,
					t_redirection_type *redir_type);
void			add_redirection_to_command(t_command *command,
					t_redirection *new_redir);
t_ast			*allocate_and_initialize_node(void);
t_command		*allocate_and_initialize_command(void);
int				handle_redirection_filename(t_parser *parser,
					t_redirection *redir);

#endif