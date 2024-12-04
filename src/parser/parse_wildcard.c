/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_wildcard.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stfn <stfn@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/04 22:19:53 by stfn              #+#    #+#             */
/*   Updated: 2024/12/04 22:20:25 by stfn             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include "minishell.h"

void	parse_wildcard(t_token *token, t_command *command)
{
	int		i;
	char	**expanded_files;

	if (token->type == TOKEN_WILDCARD)
	{
		expanded_files = expand_wildcard();
		if (!expanded_files)
		{
			ft_putstr_fd("Wildcard expansion failed\n", STDERR_FILENO);
			return ;
		}
		i = 0;
		while (command->args[i] != NULL)
		{
			add_argument_to_command(command, expanded_files[i]);
			free(expanded_files[i]);
			i++;
		}
		free(expanded_files);
	}
}

void	handle_wildcards(t_parser *parser, t_command *cmd)
{
	if (parser->current_token && parser->current_token->type == TOKEN_WILDCARD)
	{
		parse_wildcard(parser->current_token, cmd);
		parser_advance(parser);
	}
}
