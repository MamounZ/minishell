/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mang_pip_utils5.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mazaid <mazaid@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/05 16:21:35 by mazaid            #+#    #+#             */
/*   Updated: 2025/06/05 17:09:25 by mazaid           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	next_cmd(t_token **tm, t_cmd **cmd, t_ms *ms, t_token *tmp)
{
	add_cmd(ms, *cmd);
	fill_cmds(*cmd, *tm, ms);
	free_tokens(*tm);
	*tm = NULL;
	if (tmp->type == PIPE)
		*cmd = create_cmd(tmp->next, ms);
	return (1);
}

void	find_in_or_out(t_token *tmp, t_cmd *cmd, t_heredoc **t)
{
	if (tmp->type == REDIR_OUT && cmd->it_is_ok)
		open_readout(tmp, cmd);
	else if (tmp->type == REDIR_IN && cmd->it_is_ok)
		open_readin(tmp, cmd);
	else if (tmp->type == APPEND && cmd->it_is_ok)
		open_append(tmp, cmd);
	else if (tmp->type == HEREDOC)
	{
		if (cmd->fd_in != -1)
			close(cmd->fd_in);
		cmd->fd_in = (*t)->fd;
		*t = (*t)->n;
	}
}

void	cmd_error(t_cmd *cmd, t_ms *ms)
{
	free_cmds(cmd);
	ft_free_ms(ms, 1);
	exit(1);
}

void	fill_cmds_file(t_ms *ms)
{
	t_token		*tmp;
	t_cmd		*cmd;
	t_token		*tm;
	t_heredoc	*t;

	if (fill_here_doc(ms))
		return ;
	tmp = ms->tokens;
	tm = NULL;
	t = ms->doc;
	cmd = create_cmd(tmp, ms);
	while (tmp)
	{
		if (tmp->type != WORD && tmp->type != PIPE)
		{
			find_in_or_out(tmp, cmd, &t);
			tmp = tmp->next;
		}
		else if (tmp->type != PIPE && add_token(&tm, new_token
				(ft_strdup(tmp->value), tmp->type), ms) == 0)
			cmd_error(cmd, ms);
		if (tmp->type == PIPE || !tmp->next)
			next_cmd(&tm, &cmd, ms, tmp);
		tmp = tmp->next;
	}
}

void	free_cmds(t_cmd *cmds)
{
	t_cmd	*tmp;

	while (cmds)
	{
		tmp = cmds;
		cmds = cmds->next;
		free_args(tmp->args);
		free(tmp);
	}
}

// void print_args(char **args)
// {
// 	fprintf(stderr, "---------\n");
// 	int i = 0;
// 	while (args[i])
// 	{
// 		fprintf(stderr, "%s\n", args[i]);
// 		i++;
// 	}
// 	fprintf(stderr, "---------\n");
// }