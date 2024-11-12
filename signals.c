/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stfn <stfn@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/09 21:28:53 by stfn              #+#    #+#             */
/*   Updated: 2024/11/09 23:39:36 by stfn             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "signals.h"

// Function to restore original terminal settings
void	restore_terminal(t_term_context *ctx)
{
	tcsetattr(STDIN_FILENO, TCSANOW, &ctx->orig_termios);
}

// Function to disable terminal echo for Ctrl-C, Ctrl-D, and (Ctrl-\)
void	disable_echoctl(t_term_context *ctx)
{
	struct termios	new_termios;

	tcgetattr(STDIN_FILENO, &ctx->orig_termios);
	new_termios = ctx->orig_termios;
	new_termios.c_lflag &= ~(ECHOCTL);
	tcsetattr(STDIN_FILENO, TCSANOW, &new_termios);
}

// Signal handler function for Ctrl-C
void	sigint_handler(int sig)
{
	(void)sig;
	printf("\n");
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

// Function to initialize signal handling
void	handle_signals(void)
{
	struct sigaction	sa_int;

	sa_int.sa_handler = sigint_handler;
	sigemptyset(&sa_int.sa_mask);
	sa_int.sa_flags = SA_RESTART;
	sigaction(SIGINT, &sa_int, NULL);
	signal(SIGQUIT, SIG_IGN);
	signal(SIGTSTP, SIG_IGN);
}
