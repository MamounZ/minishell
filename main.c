/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaman-alrifai <yaman-alrifai@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 15:28:51 by mazaid            #+#    #+#             */
/*   Updated: 2025/06/05 19:18:09 by yaman-alrif      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	init_ms(t_ms *ms, char **argv, char **envp)
{
	ms->envp_cpy = NULL;
	ms->new_env = NULL;
	ms->tokens = NULL;
	ms->cmds = NULL;
	ms->doc = NULL;
	copy_env(envp, ms);
	ms->argv = argv;
	ms->last_exit_status = 0;
	ms->err = 0;
	ms->line_count = 1;
	setup_signals();
}

int	minshell_loop(t_ms *ms)
{
	char	*input;

	input = readline("minishell> ");
	if (!input)
	{
		ft_printf("exit\n");
		if (g_signal)
		{
			ms->last_exit_status = 130;
			g_signal = 0;
		}
		return (0);
	}
	if (*input)
		add_history(input);
	if (g_signal)
	{
		ms->last_exit_status = 130;
		g_signal = 0;
	}
	if (!(check_quotes(input)))
		ms->tokens = tokenize(input, ms);
	free(input);
	input = NULL;
	return (1);
}

int	token_alloc_good(t_ms *ms)
{
	if (ms->err == 0)
		return (0);
	ft_free_ms(ms, 1);
	exit (1);
}

void	exu_main(t_ms *ms)
{
	fill_cmds_file(ms);
	if (ms->err == 0)
	{
		if (ms && ms->cmds && !ms->cmds->next && ms->cmds->args
			&& ms->cmds->args[0] && (!ft_strcmp(ms->cmds->args[0], "cd")
				|| !ft_strcmp(ms->cmds->args[0], "export")
				|| !ft_strcmp(ms->cmds->args[0], "unset")
				|| !ft_strcmp(ms->cmds->args[0], "exit"))
			&& ms->cmds->it_is_ok)
			execute_builtin(ms->cmds->args, ms);
		else if (ms)
			exec_cmd(ms);
	}
}

int	main(int argc, char **argv, char **envp)
{
	t_ms	*ms;
	int		exit_status;

	exit_status = 0;
	(void) argc;
	ms = malloc(sizeof(t_ms));
	if (!ms)
		return (1);
	init_ms(ms, argv, envp);
	while (minshell_loop(ms))
	{
		if (token_alloc_good(ms))
			return (1);
		if (ms->tokens && !check_token(ms))
			exu_main(ms);
		ft_free_ms(ms, 0);
	}
	exit_status = ms->last_exit_status;
	ft_free_ms(ms, 1);
	return (exit_status);
}
