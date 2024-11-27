/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_redirection_heredoc.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stfn <stfn@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 10:35:42 by stfn              #+#    #+#             */
/*   Updated: 2024/11/27 10:03:36 by stfn             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include "minishell.h"

int	validate_heredoc_token(t_parser *parser, t_redirection *redir)
{
	if (!parser->current_token || parser->current_token->type != TOKEN_WORD
		|| !parser->current_token->value)
	{
		ft_print_error("Error: Expected heredoc delimiter after '<<', but found ", parser->current_token ? parser->current_token->value : "NULL", ".\n");
		free(redir);
		return (0);
	}
	redir->filename = strdup(parser->current_token->value);
	if (!redir->filename)
	{
		ft_putstr_fd("Error: Memory allocation failed for heredoc delimiter.\n", STDERR_FILENO);
		free(redir);
		return (0);
	}
	parser_advance(parser);
	return (1);
}

int	allocate_heredoc_content(t_redirection *redir, size_t content_capacity)
{
	redir->heredoc_content = malloc(content_capacity);
	if (!redir->heredoc_content)
	{
		fprintf(stderr, "Error: Memory allocation failed for heredoc content.\n");
		free(redir->filename);
		free(redir);
		return (0);
	}
	redir->heredoc_content[0] = '\0';
	return (1);
}

int	check_heredoc_delimiter(t_parser *parser, t_redirection *redir,
	int *delimiter_found)
{
	if (parser->current_token->type == TOKEN_WORD
		&& parser->current_token->value
		&& ft_strcmp(parser->current_token->value, redir->filename) == 0)
	{
		*delimiter_found = 1;
		parser_advance(parser);
		return (1);
	}
	return (0);
}

int	validate_token_value(t_parser *parser, t_redirection *redir)
{
	if (!parser->current_token->value)
	{
		fprintf(stderr, "Error: Token value is NULL in heredoc content.\n");
		free(redir->filename);
		free(redir->heredoc_content);
		free(redir);
		return (0);
	}
	return (1);
}

// int	reallocate_heredoc_content_if_needed(t_parser *parser, t_redirection *redir,
// 	size_t *content_capacity, size_t *content_length)
// {
// 	size_t	token_len;
// 	char	*new_content;

// 	token_len = ft_strlen(parser->current_token->value) + 1;
// 	if (*content_length + token_len + 1 > *content_capacity)
// 	{
// 		*content_capacity *= 2;
// 		new_content = realloc(redir->heredoc_content, *content_capacity);
// 		if (!new_content)
// 		{
// 			fprintf(stderr, "Error: Memory reallocation failed for heredoc content.\n");
// 			free(redir->filename);
// 			free(redir->heredoc_content);
// 			free(redir);
// 			return (0);
// 		}
// 		redir->heredoc_content = new_content;
// 	}
// 	return (1);
// }
int reallocate_heredoc_content_if_needed(char *line, t_redirection *redir,
    size_t *content_capacity, size_t *content_length) {
    size_t line_len = ft_strlen(line) + 1;

    if (*content_length + line_len + 1 > *content_capacity) {
        *content_capacity *= 2;
        char *new_content = realloc(redir->heredoc_content, *content_capacity);
        if (!new_content) {
            fprintf(stderr, "Error: Memory reallocation failed for heredoc content.\n");
            free(redir->filename);
            free(redir->heredoc_content);
            free(redir);
            return 0;
        }
        redir->heredoc_content = new_content;
    }
    return 1;
}

// void	append_heredoc_content(t_parser *parser, t_redirection *redir,
// 	size_t *content_length)
// {
// 	ft_strcat(redir->heredoc_content, parser->current_token->value);
// 	ft_strcat(redir->heredoc_content, "\n");
// 	*content_length += ft_strlen(parser->current_token->value) + 1;
// }
void append_heredoc_content(char *line, t_redirection *redir, size_t *content_length) {
    ft_strcat(redir->heredoc_content, line);
    ft_strcat(redir->heredoc_content, "\n");
    *content_length += ft_strlen(line) + 1;
}

int handle_missing_delimiter(t_redirection *redir)
{
	fprintf(stderr, "Error: Expected heredoc delimiter '%s' not found before end of input.\n", redir->filename);
	free(redir->filename);
	free(redir->heredoc_content);
	free(redir);
	return (0);
}
