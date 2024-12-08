/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_wildcard.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stefan <stefan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/04 22:19:53 by stfn              #+#    #+#             */
/*   Updated: 2024/12/08 23:13:16 by stefan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include "minishell.h"

/* 
 * Expands wildcard characters in the token and adds the expanded files to the 
 * command's argument list. If expansion fails, an error message is printed. 
 */
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

/* 
 * Handles wildcard tokens in the parser by expanding them and adding the 
 * results to the command's argument list. Advances the parser after processing.
 */

void	handle_wildcards(t_parser *parser, t_command *cmd)
{
	if (parser->current_token && parser->current_token->type == TOKEN_WILDCARD)
	{
		parse_wildcard(parser->current_token, cmd);
		parser_advance(parser);
	}
}
