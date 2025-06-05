/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mang_pip_utils6.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mazaid <mazaid@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/05 16:21:35 by mazaid            #+#    #+#             */
/*   Updated: 2025/06/05 17:11:10 by mazaid           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	there_are_a_douler(char *str)
{
	int	i;
	int	the_quote;

	i = 0;
	the_quote = 0;
	while (str[i])
	{
		if (!the_quote && (str[i] == '\''))
			the_quote = str[i];
		else if (str[i] == '$' && !the_quote)
			return (1);
		else if (str[i] == the_quote)
			the_quote = 0;
		i++;
	}
	return (0);
}

char	*dup_token(t_token *tmp)
{
	if (tmp->type == REDIR_IN)
		return (ft_strdup("<"));
	else if (tmp->type == REDIR_OUT)
		return (ft_strdup(">"));
	else if (tmp->type == APPEND)
		return (ft_strdup(">>"));
	else if (tmp->type == HEREDOC)
		return (ft_strdup("<<"));
	else if (tmp->type == PIPE)
		return (ft_strdup("|"));
	else
		return (ft_strdup(tmp->value));
}

void	fill_cmds_args(char **args, t_token *tm, t_ms *ms)
{
	int		i;
	t_token	*tmp;

	if (!args)
	{
		free_tokens(tm);
		ft_free_ms(ms, 1);
		exit(1);
	}
	i = 0;
	tmp = tm;
	while (tmp)
	{
		args[i++] = dup_token(tmp);
		tmp = tmp->next;
	}
	args[i] = NULL;
}

void	fill_cmds(t_cmd *cmd, t_token *tm, t_ms *ms)
{
	t_token	*tmp;
	t_token	*tmo;
	char	*input;
	char	*expanded_input;

	input = tokenize_to_char(tm);
	expanded_input = expand_variables(input, ms);
	if (!expanded_input)
		fail_expander(input, ms);
	tmp = tokenize(expanded_input, ms);
	tmo = tmp;
	free(input);
	free(expanded_input);
	rm_quote(tmp);
	cmd->args = malloc(sizeof(char *) * (token_size(tmp) + 1));
	fill_cmds_args(cmd->args, tmp, ms);
	free_tokens(tmo);
}

void	wait_all(pid_t pid, t_ms *ms)
{
	int	wstatus;
	int	last_pid;

	last_pid = wait(&wstatus);
	while (last_pid > 0)
	{
		if (last_pid == pid)
		{
			if (WIFEXITED(wstatus))
				ms->last_exit_status = WEXITSTATUS(wstatus);
			else if (WIFSIGNALED(wstatus))
				ms->last_exit_status = 128 + WTERMSIG(wstatus);
		}
		last_pid = wait(&wstatus);
	}
	setup_signals();
}
