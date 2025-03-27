/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exu.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaman-alrifai <yaman-alrifai@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 15:26:55 by yaman-alrif       #+#    #+#             */
/*   Updated: 2025/03/27 11:34:28 by yaman-alrif      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"


char *get_cmd_path(char *cmd, t_ms *ms)
{
    char *path;
    char *tmp;
    char **paths = ft_split(ft_getenv("PATH", ms), ':');
    int i = 0;
    while (paths[i])
    {
        tmp = ft_strjoin(paths[i], "/");
        path = ft_strjoin(tmp, cmd);
        free(tmp);
        // printf("path: %s\n", path);
        if (!access(path, F_OK))
        {
            free_args(paths);
            return (path);
        }
        free(path);
        i++;
    }
    free_args(paths);
    return (NULL);
}

void execute_command(t_ms *ms)
{
    char *cmd = ft_strdup("");
    char *path = NULL;
    t_token *tmp = ms->tokens;
    pid_t pid;
    int fd[2], prev_fd = -1;

    while (tmp)
    {
        if (tmp->type == PIPE || !tmp->next)
        {
            if (tmp->type != PIPE)
            {
                cmd = ft_strjoin(cmd, " ");
                path = ft_strjoin(cmd, tmp->value);
                free(cmd);
                cmd = path;
            }
            char **args = ft_split(cmd, ' ');
            if (is_builtin(args[0]))
            {
                // printf("builtin: %s\n", args[0]);
                execute_builtin(args, ms);
                free_args(args);
                free(cmd);
                cmd = ft_strdup("");
                tmp = tmp->next;
                continue;
            }
            else
            {
                if (tmp->type == PIPE && pipe(fd) == -1)
                {
                    perror("pipe");
                    exit(1);
                }
                pid = fork();
                if (pid == 0)
                {
                    if (prev_fd != -1)
                    {
                        dup2(prev_fd, STDIN_FILENO);
                        close(prev_fd);
                    }
                    if (tmp->type == PIPE)
                    {
                        close(fd[0]);
                        dup2(fd[1], STDOUT_FILENO);
                        close(fd[1]);
                    }
                    cmd = get_cmd_path(args[0], ms);
                    execve(cmd, args, ms->envp_cpy);
                    perror("execve");
                    free_args(args);
                    exit(1);
                }
                else
                {
                    if (prev_fd != -1)
                        close(prev_fd);
                    if (tmp->type == PIPE)
                    {
                        close(fd[1]);
                        prev_fd = fd[0];
                    }
                    wait(NULL);
                    free(cmd);
                    cmd = ft_strdup("");
                }
            }
        }
        else if (tmp->type == WORD)
        {
            cmd = ft_strjoin(cmd, " ");
            path = ft_strjoin(cmd, tmp->value);
            free(cmd);
            cmd = path;
        }
        tmp = tmp->next;
    }
    if (prev_fd != -1)
        close(prev_fd);
}



