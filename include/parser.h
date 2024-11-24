/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stfn <stfn@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 23:27:57 by stfn              #+#    #+#             */
/*   Updated: 2024/11/24 15:41:41 by stfn             ###   ########.fr       */
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

#endif // PARSER_H