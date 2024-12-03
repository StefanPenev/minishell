/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stfn <stfn@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/09 20:51:24 by stfn              #+#    #+#             */
/*   Updated: 2024/12/03 22:59:01 by stfn             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SIGNALS_H
# define SIGNALS_H

# define _POSIX_C_SOURCE 200809L
# define _GNU_SOURCE

# include <signal.h>
# include <termios.h>

/* ************************************************************************** */
/*                           Signal functions                                 */
/* ************************************************************************** */

// signal.c
void	reprompt(int sig);
void	sigquit(int sig);
void	interrupt(int sig);
void	sig_init(void);
void	sig_default(void);

// terminal_sig_utils.c
void	sig_ignore(void);
void	sig_heredoc(void);
void	setup_signals(void);
void	suppress_output(void);
void	restore_terminal(void);

#endif
