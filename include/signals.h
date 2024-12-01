/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stfn <stfn@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/09 20:51:24 by stfn              #+#    #+#             */
/*   Updated: 2024/12/01 16:52:46 by stfn             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SIGNALS_H
# define SIGNALS_H

# define _POSIX_C_SOURCE 200809L
# define _GNU_SOURCE

# include <signal.h>
# include <termios.h>

void	suppress_output(void);
void	reprompt(int sig);
void	sigquit(int sig);
void	interrupt(int sig);
void	sig_init(void);
void	sig_default(void);
void	sig_ignore(void);
void	sig_heredoc(void);

#endif
