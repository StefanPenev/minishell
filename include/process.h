/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anilchen <anilchen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/13 11:20:43 by anilchen          #+#    #+#             */
/*   Updated: 2024/11/20 17:09:59 by anilchen         ###   ########.fr       */
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

// control structure
// typedef struct s_ctrl
// {
// 	t_env			*env;
// 	t_process		process;
// }					t_ctrl;

/* ************************************************************************** */
/*                           Builtins commands                                */
/* ************************************************************************** */

int					execute_echo(t_command *cmd, t_env *env_copy,
						t_process *process);
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
void				swap_nodes(t_env *node1, t_env *node2);
void				minishell_lstadd_back(t_env **lst, t_env *new);
t_env				*minishell_lstnew(void *key, void *value);
// char				*ft_strjoin_three(const char *s1, const char *s2,
// 						const char *s3);
// int					minishell_lstsize(t_env *lst);

/* ************************************************************************** */
/*                                  Cleanup                                   */
/* ************************************************************************** */

void				free_splitted(char **splitted);
void				free_env(t_env **node);
void				free_node(t_env *node);

/* ************************************************************************** */
/*                       Needs to be sorted                                   */
/* ************************************************************************** */

// t_env				*create_line(char *env_str);
t_env				*init_env(char **envp);
void				my_setenv(char *key, char *value, t_env *env_copy);
char				*get_env_value(char *name, t_env *env_copy,
						t_process *process);
// void				sort_t_process_list(t_env **list);

// int						minishell_env(t_process **env_copy);
// int					minishell_env(t_env *env_copy);

// void				minishell_lstclear(t_env **node);
// void					minishell_lstclear(t_process **lst,
//						void (*del)(void *));

/* ************************************************************************** */
/*                        Manage exit satus commands                          */
/* ************************************************************************** */

void				set_exit_status(t_process *process, int status);
// void				call_exit_status(t_process *process);
// void				exit_status(t_ctrl *ctrl, int status);
// void				free_ctrl(t_ctrl *ctrl);

/* ************************************************************************** */
/*                       Execute external commands                            */
/* ************************************************************************** */

// int					execute_external_commands(char **splitted,
//						t_env *env_copy,
// 						t_process *process);

#endif