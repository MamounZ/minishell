/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exu.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaman-alrifai <yaman-alrifai@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 15:26:55 by yaman-alrif       #+#    #+#             */
/*   Updated: 2025/04/04 22:45:31 by yaman-alrif      ###   ########.fr       */
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
void print_args(char **args) {
    printf("---------\n");
    int i = 0;
    while (args[i]) {
        printf("%s\n", args[i]);
        i++;
    }
    printf("---------\n");
}
void execute_command(t_ms *ms) {
    char *cmd = ft_strdup("");
    char *path = NULL;
    t_token *tmp = ms->tokens;
    pid_t pid;
    int fd[2], prev_fd = -1;
    int stdin_copy = dup(STDIN_FILENO);
    int stdout_copy = dup(STDOUT_FILENO);
    int fd_in = -1;
    int fd_out = -1;
    
    while (tmp) {
        if (tmp->type == WORD)
        {
            path = ft_strjoin(cmd, " ");
            free(cmd);
            cmd = ft_strjoin(path, tmp->value);
            free(path);
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
            tmp = tmp->next;
        }
        if (!tmp->next || tmp->type == PIPE)
        {
            char **args = ft_split(cmd, ' ');
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
                if (is_builtin(args[0]))
                {
                    execute_builtin(args, ms);
                    free_args(args);
                    free(cmd);
                    cmd = ft_strdup("");
                }
                else 
                {
                    free(cmd);
                    cmd = get_cmd_path(args[0], ms);
                    execve(cmd, args, ms->envp_cpy);
                    perror("execve");
                    free_args(args);
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
                free_args(args);
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
                cmd = ft_strdup("");
            }
        }
        tmp = tmp->next;
    }
    
    // wait(NULL);
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
