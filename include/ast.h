/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stfn <stfn@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/13 13:17:30 by stfn              #+#    #+#             */
/*   Updated: 2024/12/03 19:58:46 by stfn             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef AST_H
# define AST_H

# include <stddef.h>

//to do some additional node types
typedef enum e_ast_node_type
{
	AST_COMMAND,
	AST_PIPELINE,
	AST_LOGICAL_AND,
	AST_LOGICAL_OR,
}	t_ast_node_type;

typedef enum e_redirection_type
{
	REDIRECT_INPUT,
	REDIRECT_OUTPUT,
	REDIRECT_APPEND,
	HEREDOC
}	t_redirection_type;

typedef struct s_redirection
{
	t_redirection_type		type;
	char					*filename;
	char					*heredoc_content;
	int						fd;
	int						was_processed;
	struct s_redirection	*next;
}	t_redirection;

typedef struct s_command
{
	char			**args;
	size_t			args_capacity;
	t_redirection	*redirections;
}	t_command;

typedef struct s_logical
{
	struct s_ast	*left;
	struct s_ast	*right;
}	t_logical;

typedef struct s_pipeline
{
	struct s_ast	*left;
	struct s_ast	*right;
}	t_pipeline;

typedef struct s_ast
{
	t_ast_node_type	type;
	union
	{
		t_command	*command;
		t_pipeline	pipeline;
		t_logical	logical;
	}	u_data;
}	t_ast;

// AST functions
void	ast_free(t_ast *ast);
void	redirection_free(t_redirection *redir);
void	command_free(t_command *cmd);

#endif
