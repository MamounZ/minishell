/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cmd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mazaid <mazaid@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/01 22:05:32 by mazaid            #+#    #+#             */
/*   Updated: 2025/06/05 14:19:22 by mazaid           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_echo(char **args)
{
	int	i;
	int	j;
	int	newline;

	i = 1;
	newline = 1;
	while (args[i] && args[i][0] == '-' && args[i][1] == 'n')
	{
		j = 1;
		while (args[i][j] == 'n')
			j++;
		if (args[i][j] != '\0')
			break ;
		newline = 0;
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

void	ft_env(t_ms *ms)
{
	int	i;

	i = 0;
	while (ms->envp_cpy[i])
	{
		ft_printf("%s\n", ms->envp_cpy[i]);
		i++;
	}
}

int	is_builtin(char *cmd)
{
	if (!cmd)
		return (0);
	if (!ft_strcmp(cmd, "echo") || !ft_strcmp(cmd, "cd")
		|| !ft_strcmp(cmd, "pwd") || !ft_strcmp(cmd, "export")
		|| !ft_strcmp(cmd, "unset") || !ft_strcmp(cmd, "env")
		|| !ft_strcmp(cmd, "exit"))
		return (1);
	return (0);
}

void	execute_builtin(char **args, t_ms *ms)
{
	ms->last_exit_status = 0;
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
	else if (ft_strcmp(args[0], "exit") == 0)
		ft_exit(args, ms);
}
