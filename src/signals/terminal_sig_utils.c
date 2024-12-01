/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   terminal_sig_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stfn <stfn@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/01 17:51:50 by stfn              #+#    #+#             */
/*   Updated: 2024/12/01 17:57:17 by stfn             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "signals.h"
#include "minishell.h"

void	suppress_output(void)
{
	struct termios	termios_p;

	if (tcgetattr(STDIN_FILENO, &termios_p) == -1)
		perror("minishell: tcgetattr");
	termios_p.c_lflag &= ~ECHOCTL;
	if (tcsetattr(STDIN_FILENO, TCSANOW, &termios_p) == -1)
		perror("minishell: tcsetattr");
}

/* Signal handler for main shell prompt */
void	reprompt(int sig)
{
	(void)sig;
	suppress_output();
	write(STDOUT_FILENO, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

/* Signal handler for sigquit */
void	sigquit(int sig)
{
	(void)sig;
	suppress_output();
	rl_on_new_line();
	rl_redisplay();
}

/* Signal handler for heredoc (child process) */
void	interrupt(int sig)
{
	(void)sig;
	suppress_output();
	write(STDOUT_FILENO, "\n", 1);
	exit(1);
}

/* Restore terminal settings */
void	restore_terminal(void)
{
	struct termios	termios_p;

	if (tcgetattr(STDIN_FILENO, &termios_p) == -1)
		perror("tcgetattr");
	termios_p.c_lflag |= ECHOCTL;
	if (tcsetattr(STDIN_FILENO, TCSANOW, &termios_p) == -1)
		perror("tcsetattr");
}
