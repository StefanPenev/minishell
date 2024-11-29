/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes_main_2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anilchen <anilchen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/29 16:12:09 by anilchen          #+#    #+#             */
/*   Updated: 2024/11/29 17:39:11 by anilchen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "process.h"

// Executes the left command in a pipeline at the given index.
// If the index is 0, it treats the command as the first command in the
// pipeline and uses `handle_first_command`. For any other index,
//	it processes the command
// as a middle command using `handle_middle_commands`.
// Logs an error message if execution fails.
// Returns EXIT_FAILURE if an error occurs, otherwise EXIT_SUCCESS.

int	execute_left_command(int index, t_command *left_cmd, t_ast *current,
		t_pipes_process_content *ctx)
{
	if (index == 0)
	{
		if (handle_first_command(current->u_data.pipeline.left,
				ctx) == EXIT_FAILURE)
		{
			printf("[ERROR] First cmd error %s\n", left_cmd->args[0]);
			return (EXIT_FAILURE);
		}
	}
	else
	{
		if (handle_middle_commands(current->u_data.pipeline.left, ctx,
				index) == EXIT_FAILURE)
		{
			printf("[ERROR] Middle cmd error %s\n", left_cmd->args[0]);
			return (EXIT_FAILURE);
		}
	}
	return (EXIT_SUCCESS);
}

// Executes the right command in a pipeline at the given index.
// If the index is less than the total number of commands,
// it treats the command as
// a middle command using `handle_middle_commands`. Otherwise,
// it processes the command
// as the last command using `handle_last_command`.
// Sets the `ctx->finish` flag if it is the last command to indicate
// pipeline completion. Logs an error message if execution fails.
// Returns EXIT_FAILURE if an error occurs, otherwise EXIT_SUCCESS.

int	execute_right_command(int index, t_command *right_cmd, t_ast *current,
		t_pipes_process_content *ctx)
{
	if (index < ctx->cmd_count - 1)
	{
		if (handle_middle_commands(current->u_data.pipeline.right, ctx,
				index) == EXIT_FAILURE)
		{
			printf("[ERROR] Middle cmd error: %s\n", right_cmd->args[0]);
			return (EXIT_FAILURE);
		}
	}
	else
	{
		if (handle_last_command(current->u_data.pipeline.right,
				ctx) == EXIT_FAILURE)
		{
			printf("[ERROR] Last cmd error: %s\n", right_cmd->args[0]);
			return (EXIT_FAILURE);
		}
		ctx->finish = 1;
	}
	return (EXIT_SUCCESS);
}

// Processes the left branch of an AST pipeline node.
// If the left branch contains a command, it executes the command
// using `execute_left_command`.
// If the left branch contains another pipeline, it recursively
// processes the nested pipeline
// using `handle_ast_pipeline`.
// Updates the `index` for commands as they are processed.
// Returns EXIT_FAILURE if an error occurs, otherwise EXIT_SUCCESS.

int	process_left_pipeline(t_ast *current, t_pipes_process_content *ctx,
		int *index)
{
	t_command	*left_cmd;

	if (current->u_data.pipeline.left)
	{
		if (current->u_data.pipeline.left->type == AST_COMMAND)
		{
			left_cmd = current->u_data.pipeline.left->u_data.command;
			if (execute_left_command(*index, left_cmd, current,
					ctx) == EXIT_FAILURE)
				return (EXIT_FAILURE);
			(*index)++;
		}
		else if (current->u_data.pipeline.left->type == AST_PIPELINE)
		{
			if (handle_ast_pipeline(current->u_data.pipeline.left,
					ctx) == EXIT_FAILURE)
				return (EXIT_FAILURE);
		}
		else
			return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}

// Processes the right branch of an AST pipeline node.
// If the right branch contains a command, it executes
// the command using `execute_right_command`.
// If the pipeline is marked as complete (`ctx->finish`), processing stops.
// Updates the `index` for commands as they are processed.
// Returns EXIT_FAILURE if an error occurs, otherwise EXIT_SUCCESS.

int	process_right_pipeline(t_ast *current, t_pipes_process_content *ctx,
		int *index)
{
	t_command	*right_cmd;

	if (current->u_data.pipeline.right
		&& current->u_data.pipeline.right->type == AST_COMMAND)
	{
		right_cmd = current->u_data.pipeline.right->u_data.command;
		if (execute_right_command(*index, right_cmd, current,
				ctx) == EXIT_FAILURE)
			return (EXIT_FAILURE);
		if (ctx->finish == 1)
			return (EXIT_SUCCESS);
		(*index)++;
	}
	return (EXIT_SUCCESS);
}

// Processes an AST representing a pipeline of commands.
// Traverses the pipeline nodes, processing the left and right branches
// using `process_left_pipeline` and `process_right_pipeline`.
// Stops processing if the pipeline is marked as complete or if an error occurs.
// Returns EXIT_FAILURE if an error occurs, otherwise EXIT_SUCCESS.

int	handle_ast_pipeline(t_ast *ast, t_pipes_process_content *ctx)
{
	t_ast		*current;
	static int	index;

	current = ast;
	index = 0;
	if (!current)
	{
		printf("[ERROR] AST is NULL.\n");
		return (EXIT_FAILURE);
	}
	while (current)
	{
		if (current->type == AST_PIPELINE)
		{
			if (process_left_pipeline(current, ctx, &index) == EXIT_FAILURE)
				return (EXIT_FAILURE);
			if (process_right_pipeline(current, ctx, &index) == EXIT_FAILURE)
				return (EXIT_FAILURE);
		}
		if (current->type == AST_PIPELINE)
			current = current->u_data.pipeline.right;
		else
			current = NULL;
	}
	return (EXIT_SUCCESS);
}
