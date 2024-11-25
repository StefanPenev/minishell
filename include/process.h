/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anilchen <anilchen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/13 11:20:43 by anilchen          #+#    #+#             */
/*   Updated: 2024/11/25 15:19:08 by anilchen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PROCESS_H
# define PROCESS_H

# include "ast.h"
# include "lexer.h"
# include "minishell.h"
# include <errno.h>
# include <limits.h>
# include <linux/limits.h>
# include <stdbool.h>
// # include <stdio.h>
// # include <stdlib.h>
# include <sys/stat.h>
# include <sys/types.h>
# include <sys/wait.h>

# define PIPE_FIRST 1
# define PIPE_MIDDLE 2
# define PIPE_LAST 3

typedef struct s_shell_context	t_shell_context;

typedef struct s_env
{
	char						*key;
	char						*value;
	struct s_env				*next;
}								t_env;

typedef struct s_pipe_fds
{
	int							fd[2];
	int							fd_prev[2];
}								t_pipe_fds;

typedef struct s_process
{
	int							last_exit_status;
}								t_process;

// typedef struct s_command_context
// {
// 	char						**env_array;
// 	t_shell_context				*shell_ctx;
// }								t_command_context;

typedef struct s_pipes_process_content
{
	t_pipe_fds					fds;
	char						**env_array;
	t_shell_context				*shell_ctx;
	t_command					*cmds[1024];
	int							cmd_count;
	pid_t						*pid;
}								t_pipes_process_content;

/* ************************************************************************** */
/*                           Builtins commands                                */
/* ************************************************************************** */

void							execute_builtin(t_command *cmd, t_env *env_copy,
									t_process *process);
int								is_builtin(t_command *cmd);
int								execute_echo(t_command *cmd,
									t_process *process);
int								execute_cd(t_command *cmd, t_env *env_copy,
									t_process *process);
int								execute_pwd(t_env *env_copy,
									t_process *process);
int								execute_export(t_command *cmd, t_env *env_copy,
									t_process *process);
int								export_no_args(t_env *env_copy);
int								export_with_args(char *key, char *value,
									t_env *env_copy);
int								execute_env(t_env *env_copy,
									t_process *process);
int								execute_unset(t_command *cmd, t_env *env_copy,
									t_process *process);
void							execute_exit(t_command *cmd,
									t_process *process);

/* ************************************************************************** */
/*                                   Utils                                    */
/* ************************************************************************** */

int								ft_strcmp(const char *s1, const char *s2);
void							minishell_lstadd_back(t_env **lst, t_env *new);
t_env							*minishell_lstnew(void *key, void *value);
char							*ft_strjoin_three(const char *s1,
									const char *s2, const char *s3);
int								minishell_lstsize(t_env *lst);
void							swap_nodes(t_env *node1, t_env *node2);
int								is_executable(const char *path);

/* ************************************************************************** */
/*                                  Cleanup                                   */
/* ************************************************************************** */

void							free_splitted(char **splitted);
void							free_env(t_env **node);
void							free_node(t_env *node);
void							cleanup_pipes_process(t_pipes_process_content *ctx);

/* ************************************************************************** */
/*                                 Enviroment                                 */
/* ************************************************************************** */

t_env							*init_env(char **envp);
void							my_setenv(char *key, char *value,
									t_env *env_copy);
char							*get_env_value(char *name, t_env *env_copy,
									t_process *process);
char							**create_env_array(t_env *env_copy);

/* ************************************************************************** */
/*                        Manage exit status commands                         */
/* ************************************************************************** */

void							set_exit_status(t_process *process, int status);

/* ************************************************************************** */
/*                       Execute external commands                            */
/* ************************************************************************** */

int								execute_external_commands(t_command *cmd,
									t_env *env_copy, t_process *process);
void							handle_child_exit_status(pid_t main_pid,
									t_process *process);

/* ************************************************************************** */
/*                              Handle path                                   */
/* ************************************************************************** */

char							*get_env(char *name, char **env_array);
char							*find_full_path(char *args, char **env_array);

/* ************************************************************************** */
/*                                   Pipes                                    */
/* ************************************************************************** */

int								main_pipes_process(t_ast *ast,
									t_shell_context *shell_ctx);
void							collect_commands_in_order(t_ast *ast,
									t_command *cmds[], int *count);
void							first_cmd(t_pipe_fds *fds, t_command *cmd,
									char **env_array,
									t_shell_context *shell_ctx);
int								process_middle_cmd(t_pipe_fds *fds, pid_t pid,
									t_command *cmd,
									t_pipes_process_content *cmd_ctx);
void							execute_middle_cmd(t_pipe_fds *fds,
									t_pipes_process_content *cmd_ctx,
									t_command *cmd);
void							last_cmd(t_pipe_fds *fds, t_command *cmd,
									char **env_array,
									t_shell_context *shell_ctx);

/* ************************************************************************** */
/*                                Pipes utils                                 */
/* ************************************************************************** */

int								initialize_pipes_process(t_ast *ast,
									t_shell_context *shell_ctx,
									t_pipes_process_content *ctx);
int								create_fork(pid_t *pid, t_process *process);
void							close_safe(int fd);
void							dup_stream(int fd, int n);
void							handle_streams(t_pipe_fds *fds, int flag);

#endif