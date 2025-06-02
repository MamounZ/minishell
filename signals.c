/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mazaid <mazaid@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 15:28:37 by mazaid            #+#    #+#             */
/*   Updated: 2025/06/02 20:00:17 by mazaid           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int g_signal = 0;

void handle_sigint(int sig)
{
	(void)sig;
	ft_printf("\n");
	rl_on_new_line();		// Move to a new line
	rl_replace_line("", 0); // Clear current input
	rl_redisplay();			// Redisplay the prompt
	g_signal = 1;			// Set the global signal variable
}

void setup_signals(void)
{
	signal(SIGINT, handle_sigint); // Handle Ctrl-C
	signal(SIGQUIT, SIG_IGN);	   // Ignore Ctrl-\ (SIGQUIT)
}

void heredoc_sigint(int sig)
{
	(void)sig;
	ft_printf("\n");
	close (0);
	g_signal = 1;
}

void setup_heredoc_signals(void)
{
	signal(SIGINT, heredoc_sigint); // Exit on Ctrl+C
	signal(SIGQUIT, SIG_IGN);		// Ignore SIGQUIT
}