/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   terminal_sig_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stefan <stefan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/01 17:51:50 by stfn              #+#    #+#             */
/*   Updated: 2024/12/08 22:57:49 by stefan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "signals.h"
#include "minishell.h"

/* 
 * Disable output control characters in the terminal.
 * This function disables the echoing of control characters in the terminal
 * by modifying the terminal settings using the termios API.
 */
void	suppress_output(void)
{
	struct termios	termios_p;

	if (tcgetattr(STDIN_FILENO, &termios_p) == -1)
		perror("minishell: tcgetattr");
	termios_p.c_lflag &= ~ECHOCTL;
	if (tcsetattr(STDIN_FILENO, TCSANOW, &termios_p) == -1)
		perror("minishell: tcsetattr");
}

/*
 * Signal handler for the main shell prompt.
 * This function is invoked when the SIGINT signal is received, typically
 * when the user presses Ctrl+C. It suppresses the output, clears the line, 
 * and re-displays the prompt.
 */
void	reprompt(int sig)
{
	(void)sig;
	suppress_output();
	write(STDOUT_FILENO, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

/*
 * Signal handler for SIGQUIT signal.
 * This function is invoked when the SIGQUIT signal is received. It suppresses
 * the output and re-displays the prompt without terminating the program.
 */
void	sigquit(int sig)
{
	(void)sig;
	suppress_output();
	rl_on_new_line();
	rl_redisplay();
}

/*
 * Signal handler for the heredoc input mode (child process).
 * This function handles the SIGINT signal in the child process during heredoc.
 * It suppresses the output, outputs a newline, and then exits with status 130.
 */
void	interrupt(int sig)
{
	(void)sig;
	suppress_output();
	write(STDOUT_FILENO, "\n", 1);
	exit(130);
}

/*
 * Restore terminal settings to the default state.
 * This function restores the terminal settings, specifically enabling echoing
 * of control characters in the terminal by modifying the terminal attributes. 
 */
void	restore_terminal(void)
{
	struct termios	termios_p;

	if (tcgetattr(STDIN_FILENO, &termios_p) == -1)
		perror("tcgetattr");
	termios_p.c_lflag |= ECHOCTL;
	if (tcsetattr(STDIN_FILENO, TCSANOW, &termios_p) == -1)
		perror("tcsetattr");
}
