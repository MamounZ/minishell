/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mang_pip_utils3.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mazaid <mazaid@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/05 16:21:35 by mazaid            #+#    #+#             */
/*   Updated: 2025/06/05 17:00:57 by mazaid           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*expand_heredoc(char *input, t_ms *ms)
{
	t_expand	e;

	ft_bzero(&e, sizeof(e));
	e.expanded = malloc(len_heredoc_exp(&e, input, ms) + 1);
	if (!e.expanded)
		return (NULL);
	e.expanded[0] = '\0';
	while (input[e.i])
	{
		if (input[e.i] == '$' && input[e.i + 1])
		{
			e.i++;
			if (handle_special_dollar_cases(input, &e, ms))
				continue ;
			if (handle_variable_expansion_heredoc(input, &e, ms))
				continue ;
			handle_plain_dollar(input, &e);
		}
		else
			ft_strncat(e.expanded, &input[e.i++], 1);
	}
	return (e.expanded);
}

void	close_and_free_heredoc(t_ms *ms)
{
	t_heredoc	*tmp;

	tmp = ms->doc;
	while (tmp)
	{
		close(tmp->fd);
		tmp = tmp->n;
	}
	free_doc(ms->doc);
	ms->doc = NULL;
}

void	exit_heredoc(t_ms *ms, char *line, int fd)
{
	close(fd);
	ft_printf("minishell: warning: here-document at line %d "
		"delimited by end-of-file (wanted `%s')\n",
		ms->line_count, ms->tokens->next->value);
	free(line);
	close_and_free_heredoc(ms);
	ft_free_ms(ms, 1);
	exit(1);
}

int	heredoc_loop(t_token *tmp, int pipe_fd[2], int expand, t_ms *ms)
{
	char	*line;
	char	*new;

	line = readline("> ");
	if (++ms->line_count && g_signal && close(pipe_fd[1]) == 0)
	{
		ft_free_ms(ms, 1);
		exit(130);
	}
	if (!line)
		exit_heredoc(ms, line, pipe_fd[1]);
	if (expand)
		new = expand_heredoc(line, ms);
	else
		new = ft_strdup(line);
	if (ft_strcmp(line, tmp->next->value) == 0 && close(pipe_fd[1]) == 0)
	{
		free(new);
		return (0);
	}
	write(pipe_fd[1], new, ft_strlen(new));
	write(pipe_fd[1], "\n", 1);
	free(new);
	free(line);
	return (1);
}

void	dad_heredoc_thing(int pipe_fd, t_ms *ms)
{
	int	status;

	waitpid(-1, &status, 0);
	if (WIFEXITED(status))
		ms->last_exit_status = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
		ms->last_exit_status = 128 + WTERMSIG(status);
	setup_signals();
	add_heredoc(ms, pipe_fd);
	if (ms->last_exit_status == 130)
	{
		ms->err = 1;
		close_and_free_heredoc(ms);
	}
}
