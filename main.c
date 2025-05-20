/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaman-alrifai <yaman-alrifai@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 15:28:51 by mazaid            #+#    #+#             */
/*   Updated: 2025/05/20 19:06:39 by yaman-alrif      ###   ########.fr       */
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
// Convert token list into an array of arguments
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

int main(int argc, char **argv, char **envp)
{
	char	*input;
	t_ms	*ms;

	(void) argv;
	(void) argc;
	ms = malloc(sizeof(t_ms));
	if (!ms)
		return (0);
	ms->envp_cpy = NULL;
	ms->new_env = NULL;
	ms->tokens = NULL;
	ms->cmds = NULL;
	ms->doc = NULL;
	ms->last_exit_status = 0;
	copy_env(envp, ms);
	setup_signals();
	ms->argv = argv;
	while (1)
	{
		input = readline("minishell> ");
		if (!input)
		{
			printf("exit\n");
			break;
		}
		if (*input)
		add_history(input);
		if (check_quotes(input))
		{
			free(input);
			continue;
		}
		if (ft_strlen(input) == 0)
		{
			free(input);
			continue;
		}
		if (g_signal)
		{
			ms->last_exit_status = 130;
			g_signal = 0;
		}
		ms->tokens = tokenize(input);
		free(input);
		input = NULL;
		if (check_token(ms))
		{
			free(input);
			continue;
		}
		fill_cmds_file(ms);
		exec_cmd(ms);
		free_tokens(ms->tokens);
		free_cmds(ms->cmds);
		ms->tokens = NULL;
		ms->cmds = NULL;
	}
	free_cmds(ms->cmds);
	free_args(ms->envp_cpy);
	free_tokens(ms->tokens);
	free(ms);
	free(input);
	return (0);
}
