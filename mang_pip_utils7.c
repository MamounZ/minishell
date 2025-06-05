/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mang_pip_utils7.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mazaid <mazaid@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/05 16:21:35 by mazaid            #+#    #+#             */
/*   Updated: 2025/06/05 17:11:56 by mazaid           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	in_out_cmds(t_cmd *tmp, int prev_fd, int fd[2])
{
	if (tmp->fd_in != -1)
		dup2(tmp->fd_in, STDIN_FILENO);
	else if (prev_fd != -1)
		dup2(prev_fd, STDIN_FILENO);
	if (tmp->fd_out != -1)
		dup2(tmp->fd_out, STDOUT_FILENO);
	else if (tmp->next)
		dup2(fd[1], STDOUT_FILENO);
	if (tmp->fd_in != -1)
		close(tmp->fd_in);
	if (tmp->fd_out != -1)
		close(tmp->fd_out);
	if (prev_fd != -1)
		close(prev_fd);
	if (tmp->next)
	{
		close(fd[0]);
		close(fd[1]);
	}
}

void	clean_child(t_cmd *tmp, int prev_fd, t_ms *ms)
{
	(void)prev_fd;
	close_cmds(tmp->next);
	ft_free_ms(ms, 1);
}

void	print_not_found(char *cmd)
{
	ft_putstr_fd("minishell: ", STDERR_FILENO);
	ft_putstr_fd(cmd, STDERR_FILENO);
	ft_putstr_fd(": command not found\n", STDERR_FILENO);
}

void	child_execve(t_cmd *tmp, int prev_fd, t_ms *ms)
{
	char	*cmd;

	close_cmds(tmp->next);
	if (tmp->args[0][0] == '/' || (tmp->args[0][0] == '.'
		&& tmp->args[0][1] == '/'))
		cmd = ft_strdup(tmp->args[0]);
	else
		cmd = get_cmd_path(tmp->args[0], ms);
	if (cmd)
	{
		execve(cmd, tmp->args, ms->envp_cpy);
		perror("execve");
		free(cmd);
	}
	else
		print_not_found(tmp->args[0]);
	clean_child(tmp, prev_fd, ms);
	exit(127);
}

void	it_is_not_ok(t_cmd *tmp, int prev_fd, int fd[2], t_ms *ms)
{
	in_out_cmds(tmp, prev_fd, fd);
	clean_child(tmp, prev_fd, ms);
	exit(1);
}
