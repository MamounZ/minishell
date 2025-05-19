/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaman-alrifai <yaman-alrifai@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 15:28:37 by mazaid            #+#    #+#             */
/*   Updated: 2025/05/19 14:46:47 by yaman-alrif      ###   ########.fr       */
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