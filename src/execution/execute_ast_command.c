/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_ast_command.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anilchen <anilchen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/04 23:32:28 by stfn              #+#    #+#             */
/*   Updated: 2024/12/06 13:54:19 by anilchen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "parser.h"

int	if_redirections(t_command *cmd, t_fd_backup *fd_backup, t_process *process)
{
	int	redirections_set;

	redirections_set = setup_redirections(cmd, fd_backup, process);
	if (redirections_set == -1)
	{
		if (process->last_exit_status == 130)
		{
			return (-1);
		}
		set_exit_status(process, 1);
		return (-1);
	}
	return (redirections_set);
}

int	check_requires_fork(t_redirection *redirections)
{
	t_redirection	*redir;

	redir = redirections;
	while (redir)
	{
		if (redir->type == HEREDOC)
			return (1);
		redir = redir->next;
	}
	return (0);
}

void	builtin_or_external(t_command *cmd, t_shell_context **shell_ctx,
		t_fd_backup *fd_backup)
{
	if (is_builtin(cmd))
		execute_builtin(cmd, (*shell_ctx)->env_copy, (*shell_ctx)->process);
	else
		execute_external_commands(cmd, (*shell_ctx)->env_copy,
			(*shell_ctx)->process);
	restore_standard_fds(fd_backup);
	return ;
}

void	make_fork_and_execute(t_command *cmd, t_shell_context **shell_ctx,
		t_fd_backup *fd_backup)
{
	pid_t			pid;
	t_redirection	*redir;

	redir = cmd->redirections;
	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		restore_standard_fds(fd_backup);
		return ;
	}
	if (pid == 0)
	{
		if (dup2(redir->fd, STDIN_FILENO) == -1)
		{
			perror("dup2");
			exit(EXIT_FAILURE);
		}
		close(redir->fd);
		execute_external_commands(cmd, (*shell_ctx)->env_copy,
			(*shell_ctx)->process);
		exit(EXIT_SUCCESS);
	}
	else
		handle_child_exit_status(pid, (*shell_ctx)->process);
}

void	execute_ast_command(t_command *cmd, t_shell_context **shell_ctx)
{
	t_fd_backup	fd_backup;
	int			requires_fork;
	int			redirections_set;

	redirections_set = 0;
	requires_fork = check_requires_fork(cmd->redirections);
	redirections_set = if_redirections(cmd, &fd_backup, (*shell_ctx)->process);
	if (redirections_set == -1)
		return ;
	if (!requires_fork)
	{
		builtin_or_external(cmd, shell_ctx, &fd_backup);
		return ;
	}
	else
		make_fork_and_execute(cmd, shell_ctx, &fd_backup);
	restore_standard_fds(&fd_backup);
}

// void	execute_ast_command(t_command *cmd, t_shell_context **shell_ctx)
// {
// 	t_fd_backup		fd_backup;
// 	pid_t			pid;
// 	t_redirection	*redir;
// 	int				requires_fork;
// 	int				redirections_set;

// 	redirections_set = 0;
// 	requires_fork = 0;
// 	redir = cmd->redirections;
// 	while (redir)
// 	{
// 		if (redir->type == HEREDOC)
// 		{
// 			requires_fork = 1;
// 			break ;
// 		}
// 		redir = redir->next;
// 	}
// 	redirections_set = setup_redirections(cmd, &fd_backup,
// 			(*shell_ctx)->process);
// 	if (redirections_set == -1)
// 	{
// 		if ((*shell_ctx)->process->last_exit_status == 130)
// 			return ;
// 		set_exit_status((*shell_ctx)->process, 1);
// 		return ;
// 	}
// 	if (is_builtin(cmd) && !requires_fork && !redirections_set)
// 	{
// 		execute_builtin(cmd, (*shell_ctx)->env_copy, (*shell_ctx)->process);
// 		restore_standard_fds(&fd_backup);
// 		return ;
// 	}
// 	if (!requires_fork)
// 	{
// 		execute_external_commands(cmd, (*shell_ctx)->env_copy,
// 			(*shell_ctx)->process);
// 		restore_standard_fds(&fd_backup);
// 		return ;
// 	}
// 	else
// 	{
// 		pid = fork();
// 		if (pid == -1)
// 		{
// 			perror("fork");
// 			restore_standard_fds(&fd_backup);
// 			return ;
// 		}
// 		if (pid == 0)
// 		{
// 			redir = cmd->redirections;
// 			while (redir)
// 			{
// 				if (redir->type == HEREDOC)
// 				{
// 					if (dup2(redir->fd, STDIN_FILENO) == -1)
// 					{
// 						perror("dup2");
// 						exit(EXIT_FAILURE);
// 					}
// 					close(redir->fd);
// 				}
// 				redir = redir->next;
// 			}
// 			execute_external_commands(cmd, (*shell_ctx)->env_copy,
// 				(*shell_ctx)->process);
// 			exit(EXIT_SUCCESS);
// 		}
// 		else
// 		{
// 			handle_child_exit_status(pid, (*shell_ctx)->process);
// 		}
// 	}
// 	restore_standard_fds(&fd_backup);
// }

// void	execute_ast_command(t_command *cmd, t_shell_context **shell_ctx)
// {
// 	t_fd_backup		fd_backup;
// 	pid_t			pid;
// 	t_redirection	*redir;
// 	int				requires_fork;
// 	int				redirections_set;

// 	redirections_set = 0;
// 	requires_fork = 0;
// 	redir = cmd->redirections;
// 	while (redir)
// 	{
// 		if (redir->type == HEREDOC)
// 		{
// 			requires_fork = 1;
// 			break ;
// 		}
// 		redir = redir->next;
// 	}
// 	redirections_set = setup_redirections(cmd, &fd_backup,
// 			(*shell_ctx)->process);
// 	if (redirections_set == -1)
// 	{
// 		if ((*shell_ctx)->process->last_exit_status == 130)
// 			return ;
// 		set_exit_status((*shell_ctx)->process, 1);
// 		return ;
// 	}
// 	if (is_builtin(cmd) && !requires_fork && !redirections_set)
// 	{
// 		execute_builtin(cmd, (*shell_ctx)->env_copy, (*shell_ctx)->process);
// 		restore_standard_fds(&fd_backup);
// 		return ;
// 	}
// 	if (!requires_fork)
// 	{
// 		execute_external_commands(cmd, (*shell_ctx)->env_copy,
// 			(*shell_ctx)->process);
// 		restore_standard_fds(&fd_backup);
// 		return ;
// 	}
// 	else
// 	{
// 		pid = fork();
// 		if (pid == -1)
// 		{
// 			perror("fork");
// 			restore_standard_fds(&fd_backup);
// 			return ;
// 		}
// 		if (pid == 0)
// 		{
// 			redir = cmd->redirections;
// 			while (redir)
// 			{
// 				if (redir->type == HEREDOC)
// 				{
// 					if (dup2(redir->fd, STDIN_FILENO) == -1)
// 					{
// 						perror("dup2");
// 						exit(EXIT_FAILURE);
// 					}
// 					close(redir->fd);
// 				}
// 				redir = redir->next;
// 			}
// 			execute_external_commands(cmd, (*shell_ctx)->env_copy,
// 				(*shell_ctx)->process);
// 			exit(EXIT_SUCCESS);
// 		}
// 		else
// 		{
// 			handle_child_exit_status(pid, (*shell_ctx)->process);
// 		}
// 	}
// 	restore_standard_fds(&fd_backup);
// }
