/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stfn <stfn@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/13 11:20:43 by anilchen          #+#    #+#             */
/*   Updated: 2024/12/05 23:51:05 by stfn             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PROCESS_H
# define PROCESS_H

# include "ast.h"
# include "lexer.h"
# include "minishell.h"
# include <ctype.h>
# include <errno.h>
# include <fcntl.h>
# include <limits.h>
# include <linux/limits.h>
# include <stdbool.h>
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

typedef struct s_pipes_process_content
{
	t_pipe_fds					fds;
	char						**env_array;
	t_shell_context				*shell_ctx;
	t_command					*cmds[1024];
	int							cmd_count;
	pid_t						*pid;
	int							finish;
}								t_pipes_process_content;

typedef struct s_fd_backup
{
	int							saved_stdin;
	int							saved_stdout;
	int							saved_stderr;
}								t_fd_backup;

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
void							node_add_back(t_env **lst, t_env *new);
t_env							*minishell_lstnew(void *key, void *value);
char							*ft_strjoin_three(const char *s1,
									const char *s2, const char *s3);
int								lstsize(t_env *lst);
void							swap_nodes(t_env *node1, t_env *node2);
int								is_executable(const char *path);
long							ft_atol(const char *str, int *overflow);

/* ************************************************************************** */
/*                                  Cleanup                                   */
/* ************************************************************************** */

void							free_splitted(char **splitted);
void							free_env(t_env **node);
void							free_node(t_env *node);
void							cleanup_pipes(t_pipes_process_content *ctx);

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
int								handle_first_command(t_ast *ast,
									t_pipes_process_content *ctx);
int								handle_middle_commands(t_ast *ast,
									t_pipes_process_content *ctx, int index);
int								handle_last_command(t_ast *ast,
									t_pipes_process_content *ctx);
void							first_cmd(t_pipe_fds *fds, t_command *cmd,
									char **env_array,
									t_shell_context *shell_ctx);
int								process_middle_cmd(t_pipe_fds *fds, pid_t *pid,
									t_command *cmd,
									t_pipes_process_content *ctx);
void							execute_middle_cmd(t_pipe_fds *fds,
									t_command *cmd,
									t_pipes_process_content *ctx);
void							last_cmd(t_pipe_fds *fds, t_command *cmd,
									char **env_array,
									t_shell_context *shell_ctx);
int								handle_ast_pipeline(t_ast *ast,
									t_pipes_process_content *ctx);
int								process_left_pipeline(t_ast *current,
									t_pipes_process_content *ctx, int *index);
int								process_right_pipeline(t_ast *current,
									t_pipes_process_content *ctx, int *index);

/* ************************************************************************** */
/*                                Pipes utils                                 */
/* ************************************************************************** */

int								initialize_pipes_process(t_ast *ast,
									t_shell_context *shell_ctx,
									t_pipes_process_content *ctx);
int								create_fork(pid_t *pid, t_process *process);
void							close_safe(int fd);
void							dup_stream(int fd, int n);
// void							handle_streams(t_pipe_fds *fds, t_command *cmd,
// 									int flag);
void							handle_streams(t_pipe_fds *fds, t_command *cmd,
									int flag, t_process *process);

/* ************************************************************************** */
/*                               Redirections                                 */
/* ************************************************************************** */
int								handle_redirections(t_command *cmd,
									t_process *process);
int								redir_output(t_redirection *redir);
int								redir_append(t_redirection *redir);
int								redir_input(t_redirection *redir);
int								process_all_heredocs(t_ast *ast,
									t_shell_context *shell_ctx);

/* ************************************************************************** */
/*                                    Heredoc                                 */
/* ************************************************************************** */
int								heredoc(t_redirection *redir,
									t_process *process);
int								handle_missing_delimiter(t_redirection *redir);
int								prepare_temp_file(char *temp_file,
									t_process *process);
int								open_temp_file(const char *temp_file);
void							process_heredoc_lines(int fd,
									t_redirection *redir);
int								process_exit_status(const char *temp_file,
									t_process *process, int status);

#endif