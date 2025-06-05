/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mang_pip.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaman-alrifai <yaman-alrifai@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/28 20:53:29 by yaman-alrif       #+#    #+#             */
/*   Updated: 2025/06/05 19:23:58 by yaman-alrif      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	it_is_okay(t_cmd *tmp, int prev_fd, int fd[2], t_ms *ms)
{
	int	e;

	in_out_cmds(tmp, prev_fd, fd);
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	if ((!tmp || !tmp->args || !tmp->args[0]))
	{
		clean_child(tmp, prev_fd, ms);
		exit(0);
	}
	else if (is_builtin(tmp->args[0]))
	{
		execute_builtin(tmp->args, ms);
		e = ms->last_exit_status;
		clean_child(tmp, prev_fd, ms);
		exit(e);
	}
	else
		child_execve(tmp, prev_fd, ms);
}

void	dad_thing(t_cmd *tmp, int *prev_fd, int fd[2])
{
	if (*prev_fd != -1)
		close(*prev_fd);
	if (tmp->next)
	{
		close(fd[1]);
		*prev_fd = fd[0];
	}
	if (tmp->fd_in != -1)
		close(tmp->fd_in);
	if (tmp->fd_out != -1)
		close(tmp->fd_out);
}

void	fail_pipe(t_ms *ms, t_cmd *tmp, int prev_fd)
{
	perror("pipe");
	if (tmp->fd_in != -1)
		close(tmp->fd_in);
	if (tmp->fd_out != -1)
		close(tmp->fd_out);
	if (prev_fd != -1)
		close(prev_fd);
	ft_free_ms(ms, 1);
	exit(1);
}

void	fail_fork(t_ms *ms, t_cmd *tmp, int prev_fd, int fd[2])
{
	perror("fork");
	if (tmp->fd_in != -1)
		close(tmp->fd_in);
	if (tmp->fd_out != -1)
		close(tmp->fd_out);
	if (prev_fd != -1)
		close(prev_fd);
	close(fd[0]);
	close(fd[1]);
	ft_free_ms(ms, 1);
	exit(1);
}

void	exec_cmd(t_ms *ms)
{
	t_cmd	*tmp;
	int		fd[2];
	int		prev_fd;
	pid_t	pid;

	tmp = ms->cmds;
	prev_fd = -1;
	while (tmp)
	{
		if (tmp->next && pipe(fd) == -1)
			fail_pipe(ms, tmp, prev_fd);
		signal(SIGINT, SIG_IGN);
		pid = fork();
		if (pid == -1)
			fail_fork(ms, tmp, prev_fd, fd);
		if (pid == 0 && tmp->it_is_ok == 0)
			it_is_not_ok(tmp, prev_fd, fd, ms);
		else if (pid == 0)
			it_is_okay(tmp, prev_fd, fd, ms);
		else
			dad_thing(tmp, &prev_fd, fd);
		tmp = tmp->next;
	}
	wait_all(pid, ms);
}
