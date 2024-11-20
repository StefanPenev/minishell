/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: spenev <spenev@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/13 11:20:43 by anilchen          #+#    #+#             */
/*   Updated: 2024/11/20 12:31:26 by spenev           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PROCESS_H
# define PROCESS_H

# include <errno.h>
# include <limits.h>
# include <linux/limits.h>
# include <stdbool.h>
# include <stdio.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <stdlib.h>

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

// int					minishell_echo(char **splitted, t_env *env_copy,
// 						t_process *process);
// int					minishell_cd(char **splitted, t_env *env_copy,
// 						t_process *process);
// int					minishell_pwd(char **splitted, t_env *env_copy,
// 						t_process *process);
// int					minishell_export(char **splitted, t_env *env_copy,
// 						t_process *process);
// // int					minishell_env(t_env *env_copy);
// int					minishell_env(char **splitted, t_env *env_copy, t_process *process);
// int					minishell_unset(char **splitted, t_env *env_copy,
// 						t_process *process);
// void				minishell_exit(char **splitted, t_env *env_copy,
// 						t_process *process);
// void				minishell_exit(char **splitted, t_env *env_copy);
// void				minishell_exit(char **splitted, t_ctrl *ctrl);

/* ************************************************************************** */
/*                                   Utils                                    */
/* ************************************************************************** */

// int					ft_strcmp(const char *s1, const char *s2);
// char				*ft_strndup(const char *s, size_t n);
void				minishell_lstadd_back(t_env **lst, t_env *new);
// t_env				*minishell_lstnew(void *key, void *value);
// char				*ft_strjoin_three(const char *s1, const char *s2,
// 						const char *s3);
// int					minishell_lstsize(t_env *lst);

/* ************************************************************************** */
/*                                  Cleanup                                   */
/* ************************************************************************** */

// void				cleanup(char **splitted);
// void				free_env(t_env **node);

/* ************************************************************************** */
/*                       Needs to be sorted                                   */
/* ************************************************************************** */

// t_env				*create_line(char *env_str);
t_env				*init_env(char **envp);
// t_process				**init_env(char **envp);
// void				my_setenv(char *key, char *value, t_env *env_copy);
// char	*get_env_value(char *name, t_env *env_copy, t_process *process);
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

// int					execute_external_commands(char **splitted, t_env *env_copy,
// 						t_process *process);

#endif