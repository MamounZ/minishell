/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exu.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaman-alrifai <yaman-alrifai@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 15:26:55 by yaman-alrif       #+#    #+#             */
/*   Updated: 2025/04/29 20:01:13 by yaman-alrif      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
char *get_cmd_path(char *cmd, t_ms *ms) {
    char *path; char *tmp; char **paths = ft_split(ft_getenv("PATH", ms), ':'); int i = 0;
    while (paths[i]) {
        tmp = ft_strjoin(paths[i], "/");
        path = ft_strjoin(tmp, cmd);
        free(tmp);
        if (!access(path, F_OK)) {
            free_args(paths);
            return (path);
        }
        free(path);
        i++;
    }
    free_args(paths);
    return (NULL);
}

char **re_aloc_new_2p(char **new, char *var)
{
    int i = 0;
    char **tmp = new;    
    
    if (!new)
    {
        tmp = malloc(sizeof(char *) * 2);
        if (!tmp) {
            perror("malloc");
            return (NULL);
        }
        tmp[0] = ft_strdup(var);
        tmp[1] = NULL;
        return (tmp);
    }
    while (new[i])
    {
        i++;
    }
    tmp = malloc(sizeof(char *) * (i + 2));
    if (!new) {
        perror("realloc");
        return (NULL);
    }
    i = 0;
    while (new[i])
    {
        tmp[i] = new[i];
        i++;
    }
    tmp[i] = ft_strdup(var);
    tmp[i + 1] = NULL;
    free(new);
    return (tmp);
}

void execute_command(t_ms *ms)
{
    char *cmd = NULL;
    char **tm = NULL;
    t_token *tmp = ms->tokens;
    pid_t pid;
    int fd[2], prev_fd = -1;
    int stdin_copy = dup(STDIN_FILENO);
    int stdout_copy = dup(STDOUT_FILENO);
    int fd_in = -1;
    int fd_out = -1;
    int co = 0;
    char **args = NULL;
    
    while (tmp) {
        if (tmp->type == WORD)
        {
            args = re_aloc_new_2p(args, tmp->value);
            tm = re_aloc_new_2p(tm, tmp->value);
        }
        else if (tmp->type == REDIR_OUT)
        {
            if (fd_out != -1)
            {
                close(fd_out);
                fd_out = -1;
            }
            fd_out = open(tmp->next->value, O_WRONLY | O_CREAT | O_TRUNC, 0644);
            if (fd_out == -1)
            {
                perror("open");
                break;
            }
            tmp = tmp->next;
        }
        else if (tmp->type == REDIR_IN)
        {
            if (fd_in != -1)
            {
                close(fd_in);
                fd_in = -1;
            }
            fd_in = open(tmp->next->value, O_RDONLY);
            if (fd_in == -1)
            {
                perror("open");
                break;
            }
            tmp = tmp->next;
        }
        else if (tmp->type == APPEND)
        {
            if (fd_out != -1)
            {
                close(fd_out);
                fd_out = -1;
            }
            fd_out = open(tmp->next->value, O_WRONLY | O_CREAT | O_APPEND, 0644);
            if (fd_out == -1)
            {
                perror("open");
                break;
            }
            tmp = tmp->next;
        }
        else if (tmp->type == HEREDOC)
        {
            int pipe_fd[2];
            if (pipe(pipe_fd) == -1)
            {
                perror("pipe");
                break;
            }
            char *line = NULL;
            size_t len = 0;
            ssize_t read_len;

            // Read lines from stdin until the delimiter is encountered
            while (1)
            {
                printf("> ");
                read_len = getline(&line, &len, stdin);
                if (read_len == -1)
                {
                    perror("getline");
                    free(line);
                    close(pipe_fd[0]);
                    close(pipe_fd[1]);
                    break;
                }
                // Remove the trailing newline character
                char *new = expand_variables(NULL, line, ms, 0);
                int si = ft_strlen(new);
                if (new[si - 1] == '\n')
                    new[si - 1] = '\0';
                if (strcmp(new, tmp->next->value) == 0)
                    break;
                write(pipe_fd[1], new, ft_strlen(new));
                write(pipe_fd[1], "\n", 1);
            }
            free(line);
            close(pipe_fd[1]);
            if (fd_in != -1)
            {
                close(fd_in);
                fd_in = -1;
            }
            fd_in = pipe_fd[0];
            tmp = tmp->next;
        }
        if (!tmp->next || tmp->type == PIPE)
        {
            if (tmp->type == PIPE)
            {
                if (pipe(fd) == -1)
                {
                    perror("pipe");
                    exit(1);
                }
                co++;
            }
            pid = fork();
            if (pid == 0)
            {
                if (fd_in != -1)
                {
                    // fprintf(stderr, "fd_in: %d\n", fd_in);
                    dup2(fd_in, STDIN_FILENO);
                    close(fd_in);
                }
                else if (prev_fd != -1)
                {
                    dup2(prev_fd, STDIN_FILENO);
                    close(prev_fd);
                }
                if (fd_out != -1)
                {
                    dup2(fd_out, STDOUT_FILENO);
                    close(fd_out);
                }
                else if (tmp->type == PIPE)
                {
                    close(fd[0]);
                    dup2(fd[1], STDOUT_FILENO);
                    close(fd[1]);
                }
                if (args)
                {
                    if (is_builtin(args[0]))
                    {
                        execute_builtin(args, ms);
                        free_args(args);
                    }
                    else 
                    {
                        
                            if (args[0][0] == '/' || (args[0][0] == '.' && args[0][1] == '/'))
                                cmd = ft_strdup(args[0]);
                            else
                                cmd = get_cmd_path(args[0], ms);
                            execve(cmd, args, ms->envp_cpy);
                            perror("execve");
                            free_args(args);
                        
                    }
                }
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
                free(cmd);
                if (args)
                {
                    free_args(args);
                    args = NULL;
                }
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
            }
            if (!co && !tmp->next && tm && (!ft_strcmp(tm[0], "cd") || !ft_strcmp(tm[0], "export") ||
            !ft_strcmp(tm[0], "unset")))
                execute_builtin(tm, ms);
            if (tm)
                free_args(tm);
            tm = NULL;
        }
        tmp = tmp->next;
    }
    int status;
    while ((wait(&status)) > 0) {
    }
    dup2(stdin_copy, STDIN_FILENO);
    dup2(stdout_copy, STDOUT_FILENO);
    if (prev_fd != -1)
        close(prev_fd);
    if (fd_in != -1)
        close(fd_in);
    if (fd_out != -1)
        close(fd_out);
    if (stdin_copy != -1)
        close(stdin_copy);
    if (stdout_copy != -1)
        close(stdout_copy);
    free(cmd);
    free_tokens(ms->tokens);
}
