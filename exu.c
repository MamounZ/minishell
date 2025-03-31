/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exu.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaman-alrifai <yaman-alrifai@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 15:26:55 by yaman-alrif       #+#    #+#             */
/*   Updated: 2025/03/31 06:23:23 by yaman-alrif      ###   ########.fr       */
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
    int stdin_copy = dup(STDIN_FILENO);
    int stdout_copy = dup(STDOUT_FILENO);
    int fd_in = -1;
    int fd_out = -1;

    while (tmp)
    {
            if (!tmp->next || tmp->type == PIPE)
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
                    execute_builtin(args, ms);
                    free_args(args);
                    free(cmd);
                    cmd = ft_strdup("");
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
                        if (fd_in != -1)
                        {
                            dup2(fd_in, STDIN_FILENO);
                            close(fd_in);
                        }
                        else if (prev_fd != -1)
                        {
                            dup2(prev_fd, STDIN_FILENO);
                            close(prev_fd);
                        }
                        else
                        {
                            dup2(stdin_copy, STDIN_FILENO);
                        }
                        if (tmp->type == PIPE)
                        {
                            if (fd_out != -1)
                            {
                                dup2(fd_out, STDOUT_FILENO);
                                close(fd_out);
                            }
                            else if (prev_fd != -1)
                            {
                                dup2(prev_fd, STDOUT_FILENO);
                                close(prev_fd);
                            }
                            else
                            {
                                dup2(stdout_copy, STDOUT_FILENO);
                            }
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
                        if (fd_out != -1)
                        {
                            close(fd_out);
                            fd_out = -1;
                        }
                        if (fd_in != -1)
                        {
                            close(fd_in);
                            fd_in = -1;
                        }
                        dup2(stdout_copy, STDOUT_FILENO);
                        dup2(stdin_copy, STDIN_FILENO);
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
        else if (tmp->type == REDIR_OUT)
        {
            fd_out = open(tmp->next->value, O_WRONLY | O_CREAT | O_TRUNC, 0644);
            if (fd_out == -1)
            {
                perror("open");
                exit(1);
            }
            dup2(fd_out, STDOUT_FILENO);
            close(fd_out);
            tmp = tmp->next;
            if (!tmp->next || tmp->next->type == PIPE)
            {
                char **args = ft_split(cmd, ' ');
                if (is_builtin(args[0]))
                {
                    execute_builtin(args, ms);
                    free_args(args);
                    free(cmd);
                    cmd = ft_strdup("");
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
                        if (fd_in != -1)
                        {
                            dup2(fd_in, STDIN_FILENO);
                            close(fd_in);
                        }
                        else if (prev_fd != -1)
                        {
                            dup2(prev_fd, STDIN_FILENO);
                            close(prev_fd);
                        }
                        else
                        {
                            dup2(stdin_copy, STDIN_FILENO);
                        }
                        if (tmp->type == PIPE)
                        {
                            if (fd_out != -1)
                            {
                                dup2(fd_out, STDOUT_FILENO);
                                close(fd_out);
                            }
                            else if (prev_fd != -1)
                            {
                                dup2(prev_fd, STDOUT_FILENO);
                                close(prev_fd);
                            }
                            else
                            {
                                dup2(stdout_copy, STDOUT_FILENO);
                            }
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
                        if (fd_out != -1)
                        {
                            close(fd_out);
                            fd_out = -1;
                        }
                        if (fd_in != -1)
                        {
                            close(fd_in);
                            fd_in = -1;
                        }
                        dup2(stdout_copy, STDOUT_FILENO);
                        dup2(stdin_copy, STDIN_FILENO);
                        cmd = ft_strdup("");
                    }
                }
            }
        }
        else if (tmp->type == REDIR_IN)
        {
            fd_in = open(tmp->next->value, O_RDONLY);
            if (fd_in == -1)
            {
                perror("open");
                exit(1);
            }
            dup2(fd_in, STDIN_FILENO);
            close(fd_in);
            tmp = tmp->next;
            if (!tmp->next || tmp->next->type == PIPE)
            {
                char **args = ft_split(cmd, ' ');
                if (is_builtin(args[0]))
                {
                    execute_builtin(args, ms);
                    free_args(args);
                    free(cmd);
                    cmd = ft_strdup("");
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
                        if (fd_in != -1)
                        {
                            dup2(fd_in, STDIN_FILENO);
                            close(fd_in);
                        }
                        else if (prev_fd != -1)
                        {
                            dup2(prev_fd, STDIN_FILENO);
                            close(prev_fd);
                        }
                        else
                        {
                            dup2(stdin_copy, STDIN_FILENO);
                        }
                        if (tmp->type == PIPE)
                        {
                            if (fd_out != -1)
                            {
                                dup2(fd_out, STDOUT_FILENO);
                                close(fd_out);
                            }
                            else if (prev_fd != -1)
                            {
                                dup2(prev_fd, STDOUT_FILENO);
                                close(prev_fd);
                            }
                            else
                            {
                                dup2(stdout_copy, STDOUT_FILENO);
                            }
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
                        if (fd_out != -1)
                        {
                            close(fd_out);
                            fd_out = -1;
                        }
                        if (fd_in != -1)
                        {
                            close(fd_in);
                            fd_in = -1;
                        }
                        dup2(stdout_copy, STDOUT_FILENO);
                        dup2(stdin_copy, STDIN_FILENO);
                        cmd = ft_strdup("");
                    }
                }
            }
        }
        tmp = tmp->next;
    }
    dup2(stdin_copy, STDIN_FILENO);
    dup2(stdout_copy, STDOUT_FILENO);
    if (prev_fd != -1)
        close(prev_fd);
}
