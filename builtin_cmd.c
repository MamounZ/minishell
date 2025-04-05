/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cmd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaman-alrifai <yaman-alrifai@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/01 22:05:32 by mazaid            #+#    #+#             */
/*   Updated: 2025/04/01 12:43:00 by yaman-alrif      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void ft_echo(char **args)
{
	int i = 1;
	int newline = 1;

	// Check for -n flag
	while (args[i] && args[i][0] == '-' && args[i][1] == 'n')
	{
		int j = 1;
		while (args[i][j] == 'n') // Ensure all characters after '-' are 'n'
			j++;
		if (args[i][j] != '\0') // If there's a non-'n' character, stop checking
			break;
		newline = 0; // Valid -n flag detected
		i++;
	}
	while (args[i])
	{
		ft_printf("%s", args[i]);
		if (args[i + 1])
			ft_printf(" ");
		i++;
	}
	if (newline)
		ft_printf("\n");
}

void ft_env(t_ms *ms)
{
	int i = 0;
	while (ms->envp_cpy[i])
	{
		ft_printf("%s\n", ms->envp_cpy[i]);
		i++;
	}
}

int is_builtin(char *cmd)
{
	if (!cmd)
		return 0;
	if (!ft_strcmp(cmd, "echo") || !ft_strcmp(cmd, "cd") ||
		!ft_strcmp(cmd, "pwd") || !ft_strcmp(cmd, "export") ||
		!ft_strcmp(cmd, "unset") || !ft_strcmp(cmd, "env") ||
		!ft_strcmp(cmd, "exit"))
		return 1;
	return 0;
}

void execute_builtin(char **args, t_ms *ms)
{
	if (!args || !args[0])
		return ;
	if (ft_strcmp(args[0], "echo") == 0)
		ft_echo(args);
	else if (ft_strcmp(args[0], "cd") == 0)
		ft_cd(args, ms);
	else if (ft_strcmp(args[0], "pwd") == 0)
		ft_pwd();
	else if (ft_strcmp(args[0], "export") == 0)
		ft_export(args, ms);
	else if (ft_strcmp(args[0], "unset") == 0)
		ft_unset(args, ms);
	else if (ft_strcmp(args[0], "env") == 0)
		ft_env(ms);
	// else if (ft_strcmp(args[0], "exit") == 0)
	// 	ft_exit(args, ms);
}
