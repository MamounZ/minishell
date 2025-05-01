/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mang_pip.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaman-alrifai <yaman-alrifai@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/28 20:53:29 by yaman-alrif       #+#    #+#             */
/*   Updated: 2025/05/01 19:49:12 by yaman-alrif      ###   ########.fr       */
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
    (void) tmp;
    t_cmd *cmd = malloc(sizeof(t_cmd));
    if (!cmd)
        return (NULL);
    cmd->args = NULL;
    cmd->path = NULL;
    cmd->fd_in = -1;
    cmd->fd_out = -1;
    cmd->next = NULL;
    cmd->it_is_ok = 1;
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
int token_size(t_token *tmp)
{
    int size = 0;
    while (tmp)
    {
        size++;
        tmp = tmp->next;
    }
    return (size);
}

void rm_quote_c(char *str)
{
    int i = 0;
    int j = 0;
    char quote = 0;

    while (str[i])
    {
        if ((str[i] == '\'' || str[i] == '\"') && !quote)
        {
            quote = str[i];
            i++;
        }
        else if (str[i] == quote)
        {
            quote = 0;
            i++;
        }
        else
        {
            str[j++] = str[i++];
        }
    }
    str[j] = '\0';
}

void fill_cmds_file(t_ms *ms)
{
    t_token *tmp = ms->tokens;
    t_cmd *cmd = create_cmd(tmp);
    t_token *tm = NULL;

    while (tmp)
    {
        if (tmp->type == REDIR_OUT || tmp->type == REDIR_IN || tmp->type == APPEND)
        {
            if (tmp->type == REDIR_OUT && cmd->it_is_ok)
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
            else if (tmp->type == REDIR_IN && cmd->it_is_ok)
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
            else if (tmp->type == APPEND && cmd->it_is_ok)
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
            tmp = tmp->next;
        }
        else if (tmp->type == HEREDOC)
        {
            rm_quote_c(tmp->next->value);
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
            // fprintf(stderr, "heredoc: %s\n", tmp->next->value);
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
                char *new = expand_variables(NULL, line, ms, 0, -1);
                int si = ft_strlen(new);
                if (new[si - 1] == '\n')
                    new[si - 1] = '\0';
                line[read_len - 1] = '\0';
                // fprintf(stderr, "new: %d\n", strcmp(line, tmp->next->value));
                if (strcmp(line, tmp->next->value) == 0)
                    break;
                write(pipe_fd[1], new, ft_strlen(new));
                write(pipe_fd[1], "\n", 1);
            }
            free(line);
            close(pipe_fd[1]);
            if (cmd->fd_in != -1)
                close(cmd->fd_in);
            cmd->fd_in = pipe_fd[0];
            tmp = tmp->next;
        }
        else if (tmp->type != PIPE)
            add_token(&tm, new_token(ft_strdup(tmp->value), tmp->type));
        if (tmp->type == PIPE || !tmp->next)
        {
            add_cmd(ms, cmd);
            fill_cmds(cmd, tm, ms);
            free_tokens(tm);
            tm = NULL;
            if (tmp->type == PIPE)
            {
                cmd = create_cmd(tmp->next);
            }
        }
        tmp = tmp->next;
    }
}

void print_args(char **args) {
    fprintf(stderr, "---------\n");
    int i = 0;
    while (args[i]) {
        fprintf(stderr, "%s\n", args[i]);
        i++;
    }
    fprintf(stderr, "---------\n");
}

void fill_cmds(t_cmd *cmd, t_token *tm, t_ms *ms)
{
    t_token *tmp = tm;
    int i = 0;
    char *input = tokenize_to_char(tm);
    char *expanded_input = expand_variables(NULL, input, ms, 0, -1);
    // fprintf(stderr, "expanded_input: %s\n", expanded_input);
    tmp = tokenize(expanded_input);
    // print_tokens(tmp);
    t_token *tmp2 = NULL, *head = tmp;
    free(input);
    free(expanded_input);
    
    while (tmp)
    {
        if (ft_strchr(tmp->value, '$'))
        {
            input = expand_variables(NULL, tmp->value, ms, 0, 0);
            t_token *t = tokenize(input);
            fprintf(stderr, "input: %s\n", tmp->value);
            fprintf(stderr, "input: %s\n", input);
            t_token *tmp3 = tmp->next;
            free(tmp->value);
            if (!tmp2)
            {
                free(tmp);
                tmp = t;
                while (t->next)
                {
                    t = t->next;
                }
                t->next = tmp3;
                head = tmp;
                tmp = tmp3;
                tmp2 = t;
            }
            else
            {
                tmp2->next = t;
                while (t->next)
                {
                    t = t->next;
                }
                t->next = tmp3;
                tmp2 = t;
                free(tmp);
                tmp = tmp3;
            }
        }
        else
        {
            tmp2 = tmp;
            rm_quote_c(tmp->value);
            tmp = tmp->next;
        }
    }
    tmp = head;
    cmd->args = malloc(sizeof(char *) * (token_size(tmp) + 1));
    while (tmp)
    {
        // fprintf(stderr, "input: %s\n", input);
        if (tmp->type == REDIR_IN)
            cmd->args[i] = ft_strdup("<");
        else if (tmp->type == REDIR_OUT)
            cmd->args[i] = ft_strdup(">");
        else if (tmp->type == APPEND)
            cmd->args[i] = ft_strdup(">>");
        else if (tmp->type == HEREDOC)
            cmd->args[i] = ft_strdup("<<");
        else if (tmp->type == PIPE)
            cmd->args[i] = ft_strdup("|");
        else
            cmd->args[i] = ft_strdup(tmp->value);
        i++;
        tmp = tmp->next;
    }
    free_tokens(tmp2);
    cmd->args[i] = NULL;
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

    if (!tmp || !tmp->args || !tmp->args[0])
    {
        if (tmp && tmp->fd_in != -1)
            close(tmp->fd_in);
        if (tmp && tmp->fd_out != -1)
            close(tmp->fd_out);
        dup2(stdin_copy, STDIN_FILENO);
        close(stdin_copy);
        dup2(stdout_copy, STDOUT_FILENO);
        close(stdout_copy);
        return ;
    }
    if (!tmp->next && (!ft_strcmp(tmp->args[0], "cd") || !ft_strcmp(tmp->args[0], "export") ||
    !ft_strcmp(tmp->args[0], "unset")) && tmp->it_is_ok)
        execute_builtin(tmp->args, ms);
    else
    {
        while (tmp)
        {
            if (tmp->it_is_ok == 0)
            {
                if (tmp->fd_in != -1)
                    close(tmp->fd_in);
                if (tmp->fd_out != -1)
                    close(tmp->fd_out);
                if (prev_fd != -1)
                    close(prev_fd);
                prev_fd = -1;
                tmp = tmp->next;
                continue;
            }
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
                if (is_builtin(tmp->args[0]))
                {
                    execute_builtin(tmp->args, ms);
                }
                else 
                {               
                    if (tmp->args[0][0] == '/' || (tmp->args[0][0] == '.' && tmp->args[0][1] == '/'))
                        cmd = ft_strdup(tmp->args[0]);
                    else
                        cmd = get_cmd_path(tmp->args[0], ms);
                    // print_args(tmp->args);
                    // printf("\n\n\n\n\n");
                    execve(cmd, tmp->args, ms->envp_cpy);
                    perror("execve");
                    free(cmd);
                }
                exit(1);
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
    }
    int status;
    while ((wait(&status)) > 0) {
    }
    dup2(stdin_copy, STDIN_FILENO);
    close(stdin_copy);
    dup2(stdout_copy, STDOUT_FILENO);
    close(stdout_copy);
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
