/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mang_pip_utils4.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mazaid <mazaid@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/05 16:21:35 by mazaid            #+#    #+#             */
/*   Updated: 2025/06/05 17:09:07 by mazaid           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	fork_heredoc(t_token *tmp, int pipe_fd[2], int expand, t_ms *ms)
{
	pid_t	pid;

	signal(SIGINT, SIG_IGN);
	pid = fork();
	if (pid < 0)
	{
		perror("fork");
		exit(1);
	}
	if (pid == 0)
	{
		close(pipe_fd[0]);
		setup_heredoc_signals();
		while (heredoc_loop(tmp, pipe_fd, expand, ms))
			;
		close_and_free_heredoc(ms);
		ft_free_ms(ms, 1);
		exit(0);
	}
	else
	{
		close(pipe_fd[1]);
		dad_heredoc_thing(pipe_fd[0], ms);
	}
}

int	fill_here_doc(t_ms *ms)
{
	t_token	*tmp;
	int		expand;
	int		pipe_fd[2];

	tmp = ms->tokens;
	expand = 1;
	while (tmp)
	{
		if (tmp->type == HEREDOC)
		{
			if (is_quote(tmp->next->value[0]) && rm_quote_c(tmp->next->value))
				expand = 0;
			if (pipe(pipe_fd) == -1)
			{
				perror("pipe");
				ft_free_ms(ms, 1);
				exit(1);
			}
			fork_heredoc(tmp, pipe_fd, expand, ms);
			if (ms->err)
				return (1);
		}
		tmp = tmp->next;
	}
	return (0);
}

void	open_readout(t_token *tmp, t_cmd *cmd)
{
	if (cmd->fd_out != -1)
		close(cmd->fd_out);
	cmd->fd_out = open(tmp->next->value, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (cmd->fd_out == -1)
	{
		perror("open");
		cmd->it_is_ok = 0;
	}
}

void	open_readin(t_token *tmp, t_cmd *cmd)
{
	if (cmd->fd_in != -1)
		close(cmd->fd_in);
	cmd->fd_in = open(tmp->next->value, O_RDONLY);
	if (cmd->fd_in == -1)
	{
		perror("open");
		cmd->it_is_ok = 0;
	}
}

void	open_append(t_token *tmp, t_cmd *cmd)
{
	if (cmd->fd_out != -1)
		close(cmd->fd_out);
	cmd->fd_out = open(tmp->next->value, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (cmd->fd_out == -1)
	{
		perror("open");
		cmd->it_is_ok = 0;
	}
}
