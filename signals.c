/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mazaid <mazaid@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 15:28:37 by mazaid            #+#    #+#             */
/*   Updated: 2025/03/12 15:35:56 by mazaid           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void handle_sigint(int sig)
{
	(void)sig;
	printf("\n");
	rl_on_new_line();		// Move to a new line
	rl_replace_line("", 0); // Clear current input
	rl_redisplay();			// Redisplay the prompt
}

void setup_signals(void)
{
	signal(SIGINT, handle_sigint); // Handle Ctrl-C
	signal(SIGQUIT, SIG_IGN);	   // Ignore Ctrl-\ (SIGQUIT)
}