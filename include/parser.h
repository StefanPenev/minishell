/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stfn <stfn@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 23:27:57 by stfn              #+#    #+#             */
/*   Updated: 2024/12/01 16:54:21 by stfn             ###   ########.fr       */
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

// Parser functions
t_ast			*parse_tokens(t_token *tokens);

// Parsing functions
t_ast			*parse_command_line(t_parser *parser);
t_ast			*parse_logical_expression(t_parser *parser);
t_ast			*parse_pipeline(t_parser *parser);
t_ast			*parse_command(t_parser *parser);

void			parse_wildcard(t_token *token, t_command *command);
void			free_redirections(t_redirection *redir);
void			parser_advance(t_parser *parser);
int				is_redirection_token(t_token_type token_type);
t_redirection	*parse_redirection(t_parser *parser);
int				map_token_to_redirection(t_token_type token_type,
					t_redirection_type *redir_type);

// parse_redirection_heredoc.c
int				validate_heredoc_token(t_parser *parser, t_redirection *redir);
int				allocate_heredoc_content(t_redirection *redir,
					size_t content_capacity);
int				check_heredoc_delimiter(t_parser *parser, t_redirection *redir,
					int *delimiter_found);
int				validate_token_value(t_parser *parser, t_redirection *redir);
void			append_heredoc_content(char *line, t_redirection *redir,
					size_t *content_length);
int				handle_missing_delimiter(t_redirection *redir);

#endif 