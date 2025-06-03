/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaman-alrifai <yaman-alrifai@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 15:28:51 by mazaid            #+#    #+#             */
/*   Updated: 2025/06/03 16:47:34 by yaman-alrif      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// checj any quotes in the input
int check_quotes(char *input)
{
	int num = 0;
	while (*input)
	{
		if (*input == '"' || *input == '\'')
		{
			int quote = *input;
			num++;
			input++;
			while (*input && *input != quote)
				input++;
			if (*input == quote)
				num++;
		}
		if (*input)
			input++;
	}
	if (num % 2)
	{
		ft_printf("minishell: syntax error: unexpected end of file\n");
		return (1);
	}
	return (0);
}

char **tokens_to_args(t_token *tokens)
{
	int count;
	int i;
	t_token *tmp;

	count = 0;
	i = 0;
	tmp = tokens;
	while (tmp)
	{
		count++;
		tmp = tmp->next;
	}
	char **args = malloc(sizeof(char *) * (count + 1));
	if (!args)
		return (NULL);
	tmp = tokens;
	while (i < count)
	{
		args[i] = ft_strdup(tmp->value);
		tmp = tmp->next;
		i++;
	}
	args[count] = NULL;
	return (args);
}

void free_args(char **args)
{
	int i = 0;

	if (!args)
		return;
	while (args[i])
		free(args[i++]);
	free(args);
}

void print_hredoc(t_heredoc *doc)
{
	t_heredoc *tmp = doc;
	while (tmp)
	{
		printf("fd: %d\n", tmp->fd);
		tmp = tmp->n;
	}
}

void ft_free_ms(t_ms *ms, int last)
{
	free_tokens(ms->tokens);
	ms->tokens = NULL;
	free_cmds(ms->cmds);
	ms->cmds = NULL;
	free_doc(ms->doc);
	ms->doc = NULL;
	ms->err = 0;
	if (last)
	{
		free_args(ms->envp_cpy);
		ms->envp_cpy = NULL;
		free_args(ms->new_env);
		ms->new_env = NULL;
		free(ms);
	}
}

void init_ms(t_ms *ms, char **argv, char **envp)
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
	setup_signals();
}

int minshell_loop(t_ms *ms)
{
	char *input;

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
		return (1);
	ft_free_ms(ms, 1);
	exit (1);
}

int main(int argc, char **argv, char **envp)
{
	t_ms	*ms;
	int exit_status;

	exit_status = 0;
	(void) argc;
	ms = malloc(sizeof(t_ms));
	if (!ms)
		return (1);
	init_ms(ms, argv, envp);
	while (minshell_loop(ms))
	{
		if (token_alloc_good(ms) && check_token(ms))
			return (1);
		fill_cmds_file(ms);
		if (ms->err == 0)
		{
			if (ms && ms->cmds && !ms->cmds->next && ms->cmds->args && ms->cmds->args[0] && (!ft_strcmp(ms->cmds->args[0], "cd") || !ft_strcmp(ms->cmds->args[0], "export") ||
    			!ft_strcmp(ms->cmds->args[0], "unset") || !ft_strcmp(ms->cmds->args[0], "exit")) && ms->cmds->it_is_ok)
        		execute_builtin(ms->cmds->args, ms);
			else if (ms)
				exec_cmd(ms);
		}
		ft_free_ms(ms, 0);
	}
	exit_status = ms->last_exit_status;
	ft_free_ms(ms, 1);
	return (exit_status);
}
