/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mang_pip.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaman-alrifai <yaman-alrifai@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/28 20:53:29 by yaman-alrif       #+#    #+#             */
/*   Updated: 2025/04/28 22:35:22 by yaman-alrif      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int num_of_words(t_token *tmp)
{
    int count = 0;
    int it_is = 0;

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

t_cmd *create_cmd(t_token *tmp)
{
    t_cmd *cmd = malloc(sizeof(t_cmd));
    if (!cmd)
        return (NULL);
    cmd->args = malloc(sizeof(char *) * (num_of_words(tmp) + 1));
    if (!cmd->args)
        return (NULL);
    cmd->path = NULL;
    cmd->fd_in = -1;
    cmd->fd_out = -1;
    cmd->next = NULL;
    return (cmd);
}

void add_cmd(t_ms *ms, t_cmd *cmd)
{
    if (!ms->cmds)
    {
        ms->cmds = cmd;
    }
    else
    {
        t_cmd *tmp = ms->cmds;
        while (tmp->next)
            tmp = tmp->next;
        tmp->next = cmd;
    }
}

void fill_cmds(t_ms *ms)
{
    t_token *tmp = ms->tokens;
    int i = 0;
    t_cmd *cmd = create_cmd(tmp);

    while (tmp)
    {
        
        if (tmp->type == WORD)
        {
            cmd->args[i] = ft_strdup(tmp->value);
            i++;
        }
        else if (tmp->type == REDIR_OUT || tmp->type == REDIR_IN || tmp->type == APPEND)
        {
            if (tmp->type == REDIR_OUT)
                cmd->fd_out = open(tmp->next->value, O_WRONLY | O_CREAT | O_TRUNC, 0644);
            else if (tmp->type == REDIR_IN)
                cmd->fd_in = open(tmp->next->value, O_RDONLY);
            else if (tmp->type == APPEND)
                cmd->fd_out = open(tmp->next->value, O_WRONLY | O_CREAT | O_APPEND, 0644);
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
                char *new = line;//expand_variables(NULL, line, ms, 0);
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
            cmd->fd_in = pipe_fd[0];
            tmp = tmp->next;
        }
        if (tmp->type == PIPE || !tmp->next)
        {
            cmd->args[i] = NULL;
            add_cmd(ms, cmd);
            i = 0;
            if (tmp->type == PIPE)
            {
                cmd = create_cmd(tmp->next);
            }
        }
        tmp = tmp->next;
    }
}

void exec_cmd(t_ms *ms)
{
    t_cmd *tmp = ms->cmds;
    int fd[2];
    int prev_fd = -1;
    char *cmd = NULL;
    pid_t pid;
    int stdin_copy = dup(STDIN_FILENO);
    int stdout_copy = dup(STDOUT_FILENO);
    
    while (tmp)
    {
        if (tmp->next && pipe(fd) == -1)
        {
            perror("pipe");
            exit(1);
        }
        pid = fork();
        if (pid == 0)
        {
            if (tmp->fd_in != -1)
            {
                dup2(tmp->fd_in, STDIN_FILENO);
                close(tmp->fd_in);
            }
            else if (prev_fd != -1)
            {
                dup2(prev_fd, STDIN_FILENO);
                close(prev_fd);
            }
            if (tmp->fd_out != -1)
            {
                dup2(tmp->fd_out, STDOUT_FILENO);
                close(tmp->fd_out);
            }
            else if (tmp->next)
            {
                close(fd[0]);
                dup2(fd[1], STDOUT_FILENO);
                close(fd[1]);
            }
            cmd = get_cmd_path(tmp->args[0], ms);
            // fprintf(stderr, "Command: %s\n", cmd);
            execve(cmd, tmp->args, ms->envp_cpy);
            perror("execve");
            free(cmd);
        }
        else
        {
            if (prev_fd != -1)
                close(prev_fd);
            if (tmp->next)
            {
                close(fd[1]);
                prev_fd = fd[0];
            }
            if (tmp->fd_in != -1)
                close(tmp->fd_in);
            if (tmp->fd_out != -1)
                close(tmp->fd_out);
        }
        tmp = tmp->next;
    }
    int status;
    while ((wait(&status)) > 0) {
    }
    dup2(stdin_copy, STDIN_FILENO);
    dup2(stdout_copy, STDOUT_FILENO);
}

void free_cmds(t_cmd *cmds)
{
    t_cmd *tmp;
    while (cmds)
    {
        tmp = cmds;
        cmds = cmds->next;
        free_args(tmp->args);
        free(tmp);
    }
}
