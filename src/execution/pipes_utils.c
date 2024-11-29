/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anilchen <anilchen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 14:36:21 by anilchen          #+#    #+#             */
/*   Updated: 2024/11/29 17:22:48 by anilchen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "process.h"

// Duplicates a file descriptor to a specific stream (e.g., stdin, stdout,
// stderr). Uses dup2 to redirect the given file descriptor (fd) to the
// target stream (n). If dup2 fails,
// it prints an error message and exits the program with a failure status.
// Parameters:
//   - fd: The source file descriptor to duplicate.
//   - n: The target file descriptor to overwrite (e.g., STDIN_FILENO,
//	STDOUT_FILENO).
// Notes:
//   - If fd is negative,
//	the function does nothing (no redirection is performed).
//   - If dup2 fails, perror is used to log the error,
//	and the program exits immediately to avoid undefined behavior.

void	dup_stream(int fd, int n)
{
	if (fd >= 0)
	{
		if (dup2(fd, n) == -1)
		{
			perror("dup2 failed");
			exit(EXIT_FAILURE);
		}
	}
}

// Allocates memory for storing process IDs for all commands in the pipeline.
// Initializes all process IDs to -1 to indicate that they are not yet assigned.
// Parameters:
//   - ctx: Pointer to the pipeline process context,
// which contains metadata about the pipeline.
// Returns:
//   - EXIT_SUCCESS: If memory allocation and initialization are successful.
//   - EXIT_FAILURE: If memory allocation fails,
// logs the error and frees allocated resources.

int	assign_pid(t_pipes_process_content *ctx)
{
	int	i;

	i = 0;
	ctx->pid = malloc(sizeof(pid_t) * ctx->cmd_count);
	if (!ctx->pid)
	{
		perror("malloc");
		free_splitted(ctx->env_array);
		return (EXIT_FAILURE);
	}
	while (i < ctx->cmd_count)
	{
		ctx->pid[i] = -1;
		i++;
	}
	return (EXIT_SUCCESS);
}

// Traverses the AST (Abstract Syntax Tree) to collect all commands in execution
// order. Populates the provided `cmds` array with pointers to the commands
// and increments the count.
// Parameters:
//   - ast: Pointer to the current AST node.
//   - cmds: Array to store the collected commands in order of execution.
//
//	- count: Pointer to an integer that tracks the number of collected commands.
// Notes:
//   - Recursively traverses the AST,
//	processing commands in the left-to-right order of the pipeline.

void	collect_commands_in_order(t_ast *ast, t_command *cmds[], int *count)
{
	if (ast == NULL)
		return ;
	if (ast->type == AST_COMMAND)
	{
		cmds[*count] = ast->u_data.command;
		(*count)++;
	}
	else if (ast->type == AST_PIPELINE)
	{
		collect_commands_in_order(ast->u_data.pipeline.left, cmds, count);
		collect_commands_in_order(ast->u_data.pipeline.right, cmds, count);
	}
}

// Counts the total number of commands in the AST (Abstract Syntax Tree).
// Recursively traverses the AST to sum up all command nodes.
// Parameters:
//   - ast: Pointer to the current AST node.
// Returns:
//   - The total number of commands in the AST.

int	count_commands_in_ast(t_ast *ast)
{
	if (!ast)
		return (0);
	if (ast->type == AST_COMMAND)
		return (1);
	if (ast->type == AST_PIPELINE)
		return (count_commands_in_ast(ast->u_data.pipeline.left)
			+ count_commands_in_ast(ast->u_data.pipeline.right));
	return (0);
}

// Initializes the pipeline process context for executing the commands
// in the AST. Performs the following steps:
//   1. Counts the total number of commands in the AST.
//   2. Allocates memory for storing process IDs for each command.
//   3. Collects all commands from the AST in execution order.
//   4. Creates an environment variable array for the process context.
//   5. Initializes all pipe-related file descriptors to -1.
// Parameters:
//   - ast: Pointer to the AST representing the pipeline.
//   - shell_ctx: Pointer to the shell context, which contains global state.
//   - ctx: Pointer to the pipeline process context to initialize.
// Returns:
//   - EXIT_SUCCESS: If all initialization steps are successful.
//   - EXIT_FAILURE: If memory allocation fails at any step.

int	initialize_pipes_process(t_ast *ast, t_shell_context *shell_ctx,
		t_pipes_process_content *ctx)
{
	int	count;

	count = 0;
	ctx->finish = 0;
	ctx->pid = NULL;
	ctx->cmd_count = count_commands_in_ast(ast);
	assign_pid(ctx);
	collect_commands_in_order(ast, ctx->cmds, &count);
	ctx->env_array = create_env_array(shell_ctx->env_copy);
	if (!ctx->env_array)
	{
		perror("malloc");
		return (EXIT_FAILURE);
	}
	ctx->shell_ctx = shell_ctx;
	ctx->fds.fd[0] = -1;
	ctx->fds.fd[1] = -1;
	ctx->fds.fd_prev[0] = -1;
	ctx->fds.fd_prev[1] = -1;
	return (EXIT_SUCCESS);
}
