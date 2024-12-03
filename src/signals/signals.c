/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stfn <stfn@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/09 21:28:53 by stfn              #+#    #+#             */
/*   Updated: 2024/12/03 20:16:08 by stfn             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "signals.h"
#include "minishell.h"

/*Initialize custom signal handlers for SIGINT and SIGQUIT.
SIGINT is handled by the `reprompt` function to re-prompt the user
(e.g., in a shell).
SIGQUIT is handled by the `sigquit` function, allowing custom behavior for
quit signals.*/
void	sig_init(void)
{
	signal(SIGINT, reprompt);
	signal(SIGQUIT, sigquit);
}

/*Restore default signal handling behavior for SIGINT and SIGQUIT.
This sets the signal actions for both signals back to their default handlers.*/
void	sig_default(void)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
}

/*Ignore SIGINT and SIGQUIT signals.
This function prevents the program from reacting to these signals,
often used when you want to disable interruption during specific operations.*/
void	sig_ignore(void)
{
	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
}

/*Set signal handling for heredoc input mode.
SIGINT is handled by the `interrupt` function to allow interruption of
heredoc input.
SIGQUIT is ignored to prevent unwanted termination during heredoc input.*/
void	sig_heredoc(void)
{
	signal(SIGINT, interrupt);
	signal(SIGQUIT, SIG_IGN);
}

// Initialize signal handling for the shell.
void	setup_signals(void)
{
	sig_init();
	suppress_output();
}
