/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mang_pip_utils1.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mazaid <mazaid@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/05 16:21:35 by mazaid            #+#    #+#             */
/*   Updated: 2025/06/05 16:52:58 by mazaid           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	num_of_words(t_token *tmp)
{
	int	count;
	int	it_is;

	count = 0;
	it_is = 0;
	while (tmp && tmp->type != PIPE)
	{
		if (tmp->type != WORD)
			it_is++;
		if (it_is && tmp->type == WORD)
			count++;
		tmp = tmp->next;
	}
	return (count - it_is);
}

t_cmd	*create_cmd(t_token *tmp, t_ms *ms)
{
	t_cmd	*cmd;

	(void) tmp;
	cmd = malloc(sizeof(t_cmd));
	if (!cmd)
	{
		ft_free_ms(ms, 1);
		exit(1);
	}
	cmd->args = NULL;
	cmd->path = NULL;
	cmd->fd_in = -1;
	cmd->fd_out = -1;
	cmd->next = NULL;
	cmd->it_is_ok = 1;
	return (cmd);
}

void	fail_expander(char *input, t_ms *ms)
{
	ft_printf("minishell: syntax error near unexpected token `%s'\n", input);
	free(input);
	ft_free_ms(ms, 1);
	exit(1);
}

void	close_cmds(t_cmd *cmds)
{
	while (cmds)
	{
		if (cmds->fd_in != -1)
			close(cmds->fd_in);
		if (cmds->fd_out != -1)
			close(cmds->fd_out);
		cmds->fd_in = -1;
		cmds->fd_out = -1;
		cmds = cmds->next;
	}
}

void	add_cmd(t_ms *ms, t_cmd *cmd)
{
	t_cmd	*tmp;

	if (!ms->cmds)
	{
		ms->cmds = cmd;
	}
	else
	{
		tmp = ms->cmds;
		while (tmp->next)
			tmp = tmp->next;
		tmp->next = cmd;
	}
}
