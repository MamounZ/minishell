/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cmd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mazaid <mazaid@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/01 22:05:32 by mazaid            #+#    #+#             */
/*   Updated: 2025/03/01 22:30:54 by mazaid           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void ft_echo(char **args)
{
	int i = 1;
	int newline = 1; // Default: print newline

	// Check for -n flag
	if (args[i] && strcmp(args[i], "-n") == 0)
	{
		newline = 0;
		i++;
	}

	// Print remaining arguments with spacing
	while (args[i])
	{
		ft_printf("%s", args[i]);
		if (args[i + 1])
			printf(" ");
		i++;
	}

	// Print newline if -n was not used
	if (newline)
		printf("\n");
}

void execute_builtin(char **args, t_ms *ms)
{
	(void)ms;
	if (strcmp(args[0], "echo") == 0)
		ft_echo(args);
	// else if (strcmp(args[0], "cd") == 0)
	// 	ft_cd(args, ms);
	// else if (strcmp(args[0], "pwd") == 0)
	// 	ft_pwd();
	// else if (strcmp(args[0], "export") == 0)
	// 	ft_export(args, ms);
	// else if (strcmp(args[0], "unset") == 0)
	// 	ft_unset(args, ms);
	// else if (strcmp(args[0], "env") == 0)
	// 	ft_env(ms);
	// else if (strcmp(args[0], "exit") == 0)
	// 	ft_exit(args, ms);
	else
		printf("%s: command not found\n", args[0]);
}
