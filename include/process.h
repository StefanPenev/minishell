/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anilchen <anilchen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/13 11:20:43 by anilchen          #+#    #+#             */
/*   Updated: 2024/11/21 16:47:39 by anilchen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PROCESS_H
# define PROCESS_H

# include "minishell.h"
# include <errno.h>
# include <limits.h>
# include <linux/limits.h>
# include <stdbool.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/types.h>
# include <sys/wait.h>

typedef struct s_env
{
	char			*key;
	char			*value;
	struct s_env	*next;
}					t_env;

typedef struct s_process
{
	int				last_exit_status;
}					t_process;

typedef struct s_pipe_fds
{
	int				fd[2];
	int				fd_prev[2];
}					t_pipe_fds;

// control structure
// typedef struct s_ctrl
// {
// 	t_env			*env;
// 	t_process		process;
// }					t_ctrl;

/* ************************************************************************** */
/*                           Builtins commands                                */
/* ************************************************************************** */

void				execute_builtin(t_command *cmd, t_env *env_copy,
						t_process *process);
int					is_builtin(t_command *cmd);
int					execute_echo(t_command *cmd, t_process *process);
int					execute_cd(t_command *cmd, t_env *env_copy,
						t_process *process);
int					execute_pwd(t_env *env_copy, t_process *process);
int					execute_export(t_command *cmd, t_env *env_copy,
						t_process *process);
int					export_no_args(t_env *env_copy);
int					export_with_args(char *key, char *value, t_env *env_copy);
int					execute_env(t_env *env_copy, t_process *process);
int					execute_unset(t_command *cmd, t_env *env_copy,
						t_process *process);
void				execute_exit(t_command *cmd, t_process *process);

/* ************************************************************************** */
/*                                   Utils                                    */
/* ************************************************************************** */

int					ft_strcmp(const char *s1, const char *s2);
void				minishell_lstadd_back(t_env **lst, t_env *new);
t_env				*minishell_lstnew(void *key, void *value);
char				*ft_strjoin_three(const char *s1, const char *s2,
						const char *s3);
int					minishell_lstsize(t_env *lst);
void				swap_nodes(t_env *node1, t_env *node2);
int					is_executable(const char *path);

/* ************************************************************************** */
/*                                  Cleanup                                   */
/* ************************************************************************** */

void				free_splitted(char **splitted);
void				free_env(t_env **node);
void				free_node(t_env *node);

/* ************************************************************************** */
/*                                 Enviroment                                 */
/* ************************************************************************** */

t_env				*init_env(char **envp);
void				my_setenv(char *key, char *value, t_env *env_copy);
char				*get_env_value(char *name, t_env *env_copy,
						t_process *process);
char				**create_env_array(t_env *env_copy);

/* ************************************************************************** */
/*                        Manage exit status commands                         */
/* ************************************************************************** */

void				set_exit_status(t_process *process, int status);

/* ************************************************************************** */
/*                       Execute external commands                            */
/* ************************************************************************** */

int					execute_external_commands(t_command *cmd, t_env *env_copy,
						t_process *process);

/* ************************************************************************** */
/*                              Handle path                                   */
/* ************************************************************************** */

char				*get_env(char *name, char **env_array);
char				*find_full_path(char *args, char **env_array);

#endif