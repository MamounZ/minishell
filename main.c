/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mazaid <mazaid@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 15:28:51 by mazaid            #+#    #+#             */
/*   Updated: 2025/02/26 19:05:52 by mazaid           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int main(int argc, char **argv, char **envp)
{
	char	*input;
	t_ms	*ms;

	(void) argv;
	(void) argc;
	ms = malloc(sizeof(t_ms));
	if (!ms)
		return (0);
	while (1)
	{
		input = readline("minishell> ");
		if (!input)
		{
			printf("exit\n");
			break;
		}
		if (input)
			add_history(input);
		copy_env(envp, ms);
		printf("You entered: %s\n", input);
		free(input);
	}
	return (0);
}