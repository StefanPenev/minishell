/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stfn <stfn@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/09 20:51:24 by stfn              #+#    #+#             */
/*   Updated: 2024/11/27 10:09:41 by stfn             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SIGNALS_H
# define SIGNALS_H

# define _POSIX_C_SOURCE 200809L
# define _GNU_SOURCE

# include <signal.h>
# include <termios.h>

typedef struct s_term_context
{
	struct termios	orig_termios;
}					t_term_context;

void	handle_signals(void);
void	disable_echoctl(t_term_context *ctx);
void	restore_terminal(t_term_context *ctx);

#endif
