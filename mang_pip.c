/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mang_pip.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaman-alrifai <yaman-alrifai@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/28 20:53:29 by yaman-alrif       #+#    #+#             */
/*   Updated: 2025/06/03 21:16:05 by yaman-alrif      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void add_heredoc(t_ms *ms, int fd)
{
    t_heredoc *new;
    t_heredoc *tmp;

    new = malloc(sizeof(t_heredoc));
    if (!new)
    {
        ft_free_ms(ms, 1);
        exit (1);
    }
    tmp = ms->doc;
    new->fd = fd;
    new->n = NULL;
    if (!(tmp))
    {
        ms->doc = new;
        return ;
    }
    while(tmp->n)
    {
        tmp = tmp->n;
    }
    tmp->n = new;
}

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

t_cmd *create_cmd(t_token *tmp, t_ms *ms)
{
    (void) tmp;
    t_cmd *cmd = malloc(sizeof(t_cmd));
    if (!cmd)
    {
        ft_free_ms(ms, 1);
        exit (1);
    }
    cmd->args = NULL;
    cmd->path = NULL;
    cmd->fd_in = -1;
    cmd->fd_out = -1;
    cmd->next = NULL;
    cmd->it_is_ok = 1;
    return (cmd);
}
void fail_expander(char *input, t_ms *ms)
{
    ft_printf("minishell: syntax error near unexpected token `%s'\n", input);
    free(input);
    ft_free_ms(ms, 1);
    exit(1);
}

void close_cmds(t_cmd *cmds)
{
    while (cmds)
    {
        if (cmds->fd_in != -1)
            close(cmds->fd_in);
        if (cmds->fd_out != -1)
            close(cmds->fd_out);
        cmds->fd_in = -1;
        cmds->fd_out = -1;
        cmds = cmds->next;
    }
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

void free_doc(t_heredoc *ms)
{
    t_heredoc *tmp;
    while (ms)
    {
        tmp = ms;
        ms = ms->n;
        // close(tmp->fd);
        free(tmp);
    }
}

int rm_quote_c(char *str)
{
    int i;
    int j;
    char quote;

    i = 0;
    j = 0;
    quote = 0;
    while (str[i])
    {
        if ((str[i] == '\'' || str[i] == '\"') && !quote)
        quote = str[i];
        else if (str[i] == quote)
            quote = 0;
        else
            str[j++] = str[i];
        i++;
    }
    str[j] = '\0';
    return (1);
}

int handle_variable_expansion_heredoc(char *input, t_expand *e, t_ms *ms)
{
    if (is_valid_var_char(input[e->i]))
    {
        e->j = 0;
        while (is_valid_var_char(input[e->i]))
            e->var_name[e->j++] = input[e->i++];
        e->var_name[e->j] = '\0';
        e->value = ft_getenv(e->var_name, ms);
        if (e->value)
            ft_strncat(e->expanded, &e->value[0], ft_strlen(e->value));
        ft_memset(e->var_name, 0, sizeof(e->var_name));
        return (1);
    }
    return (0);
}
void handle_plain_dollar(char *input, t_expand *e)
{
    ft_strncat(e->expanded, "$", 1);
    if (input[e->i])
        ft_strncat(e->expanded, &input[e->i++], 1);
}

char	*expand_heredoc(char *input, t_ms *ms)
{
    t_expand e;

    ft_bzero(&e, sizeof(e));
    e.expanded = malloc(10000);
    if (!e.expanded)
        return NULL;
    e.expanded[0] = '\0';
    while (input[e.i])
    {
        if (input[e.i] == '$' && input[e.i + 1])
        {
            e.i++;
            if (handle_special_dollar_cases(input, &e, ms))
                continue;
            if (handle_variable_expansion_heredoc(input, &e, ms))
                continue;
            handle_plain_dollar(input, &e);
        }
		else
            ft_strncat(e.expanded, &input[e.i++], 1);
    }
	return (e.expanded);
}

void exit_heredoc(t_ms *ms, char *line)
{
    perror("getline");
    free(line);
    ft_free_ms(ms, 1);
    exit (1);
}

int heredoc_loop(t_token *tmp, int pipe_fd[2], int expand, t_ms *ms)
{
    char    *line;
    char    *new;

    line = readline("> ");
    if (g_signal && close(pipe_fd[0]) == 0 && close(pipe_fd[1]) == 0)
    {
        ft_free_ms(ms, 1);
        exit(130);
    }
    if (!line && close(pipe_fd[0]) == 0 && close(pipe_fd[1]) == 0)
        exit_heredoc(ms, line);
    if (expand)
        new = expand_heredoc(line, ms);
    else
        new = ft_strdup(line);
    if (ft_strcmp(line, tmp->next->value) == 0)
    {
        free(new);
        return 0;
    }
    write(pipe_fd[1], new, ft_strlen(new));
    write(pipe_fd[1], "\n", 1);
    free(new);
    free(line);
    return 1;
}

void close_and_free_heredoc(t_ms *ms)
{
    t_heredoc *tmp;

    tmp = ms->doc;
    while (tmp)
    {
        close(tmp->fd);
        tmp = tmp->n;
    }
    free_doc(ms->doc);
    ms->doc = NULL;
}

void dad_heredoc_thing(int pipe_fd[2], t_ms *ms)
{
    int status;
    
    waitpid(-1, &status, 0);
    if (WIFEXITED(status))
        ms->last_exit_status = WEXITSTATUS(status);
    else if (WIFSIGNALED(status))
        ms->last_exit_status = 128 + WTERMSIG(status);
    close(pipe_fd[1]);
    setup_signals();
    add_heredoc(ms, pipe_fd[0]);
    if (ms->last_exit_status == 130)
    {
        ms->err = 1;
        close_and_free_heredoc(ms);
    }
}

void fork_heredoc(t_token *tmp, int pipe_fd[2], int expand, t_ms *ms)
{
    pid_t pid;

    signal(SIGINT, SIG_IGN);
    pid = fork();
    if (pid < 0)
    {
        perror("fork");
        exit(1);
    }
    if (pid == 0)
    {
        setup_heredoc_signals(); /*mamoon signal*/
            while (heredoc_loop(tmp, pipe_fd, expand, ms));
        close(pipe_fd[1]);
        close(pipe_fd[0]);
        ft_free_ms(ms, 1);
        exit (0);
    }
    else
        dad_heredoc_thing(pipe_fd, ms);
}

void fill_here_doc(t_ms *ms)
{
    t_token *tmp;
    int expand;
    int pipe_fd[2];

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
                exit (1);
            }
            fork_heredoc(tmp, pipe_fd, expand, ms);
            if (ms->err)
                return;
        }
        tmp = tmp->next;
    }
}

void open_readout(t_token *tmp, t_cmd *cmd)
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

void open_readin(t_token *tmp, t_cmd *cmd)
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

void open_append(t_token *tmp, t_cmd *cmd)
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

int next_cmd(t_token **tm, t_cmd **cmd, t_ms *ms, t_token *tmp)
{
    add_cmd(ms, *cmd);
    fill_cmds(*cmd, *tm, ms);
    free_tokens(*tm);
    *tm = NULL;
    if (tmp->type == PIPE)
        *cmd = create_cmd(tmp->next, ms);
    return (1);
}

void find_in_or_out(t_token *tmp, t_cmd *cmd, t_heredoc **t)
{
    if (tmp->type == REDIR_OUT && cmd->it_is_ok)
        open_readout(tmp, cmd);
    else if (tmp->type == REDIR_IN && cmd->it_is_ok)
        open_readin(tmp, cmd);
    else if (tmp->type == APPEND && cmd->it_is_ok)
        open_append(tmp, cmd);
    else if (tmp->type == HEREDOC)
    {
        cmd->fd_in = (*t)->fd;
        *t = (*t)->n;
    }
}

void fill_cmds_file(t_ms *ms)
{
    t_token *tmp;
    t_cmd *cmd;
    t_token *tm;
    t_heredoc *t;

    fill_here_doc(ms);
    if (ms->err)
        return ;
    tmp = ms->tokens;
    tm = NULL;
    t = ms->doc;
    cmd = create_cmd(tmp, ms);
    while (tmp)
    {
        if (tmp->type == REDIR_OUT || tmp->type == REDIR_IN || tmp->type == APPEND || tmp->type == HEREDOC)
        {
            find_in_or_out(tmp, cmd, &t);
            tmp = tmp->next;
        }
        else if (tmp->type != PIPE && add_token(&tm, new_token(ft_strdup(tmp->value), tmp->type), ms) == 0)
        {
            free_cmds(cmd);
            ft_free_ms(ms, 1);
            exit(1);
        }
        if (tmp->type == PIPE || !tmp->next)
            next_cmd(&tm, &cmd, ms, tmp);
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

int there_are_a_douler(char *str)
{
    int i = 0;
    int the_quote = 0;

    while (str[i])
    {
        if (!the_quote && (str[i] == '\''))
            the_quote = str[i];
        else if (str[i] == '$' && !the_quote)
            return (1);
        else if (str[i] == the_quote)
            the_quote = 0;
        i++;
    }
    return (0);
}

char *dup_token(t_token *tmp)
{
    if (tmp->type == REDIR_IN)
        return (ft_strdup("<"));
    else if (tmp->type == REDIR_OUT)
        return (ft_strdup(">"));
    else if (tmp->type == APPEND)
        return (ft_strdup(">>"));
    else if (tmp->type == HEREDOC)
        return (ft_strdup("<<"));
    else if (tmp->type == PIPE)
        return (ft_strdup("|"));
    else
        return (ft_strdup(tmp->value));
}

void fill_cmds_args(char **args, t_token *tm, t_ms *ms)
{
    int i;
    t_token *tmp;

    if (!args)
    {
        free_tokens(tm);
        ft_free_ms(ms, 1);
        exit(1);
    }
    i = 0;
    tmp = tm;
    while (tmp)
    {
        args[i++] = dup_token(tmp);
        tmp = tmp->next;
    }
    args[i] = NULL;
}

void fill_cmds(t_cmd *cmd, t_token *tm, t_ms *ms)
{
    t_token *tmp;
    t_token *tmo;
    char *input;
    char *expanded_input;

    input = tokenize_to_char(tm);
    expanded_input = expand_variables(input, ms);
    if (!expanded_input)
        fail_expander(input, ms);
    tmp = tokenize(expanded_input, ms);
    tmo = tmp;
    free(input);
    free(expanded_input);
    rm_quote(tmp);
    cmd->args = malloc(sizeof(char *) * (token_size(tmp) + 1));
    fill_cmds_args(cmd->args, tmp, ms);
    free_tokens(tmo);
}

void wait_all(pid_t pid, t_ms *ms)
{
    int	wstatus;
	int	last_pid;

	last_pid = wait(&wstatus);
	while (last_pid > 0)
	{
		if (last_pid == pid)
		{
			if (WIFEXITED(wstatus))
				ms->last_exit_status = WEXITSTATUS(wstatus);
			else if (WIFSIGNALED(wstatus))
				ms->last_exit_status = 128 + WTERMSIG(wstatus);
		}
		last_pid = wait(&wstatus);
	}
    setup_signals();
}

void in_out_cmds(t_cmd *tmp, int prev_fd, int fd[2])
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
        close (tmp->fd_out);
    if (prev_fd != -1)
        close (prev_fd);
    if (tmp->next)
    {
        close(fd[0]);
        close(fd[1]);
    }
}

void clean_child(t_cmd *tmp, int prev_fd, t_ms *ms)
{
    (void) prev_fd;
    close_cmds(tmp->next);
    ft_free_ms(ms, 1);
}

void    child_execve(t_cmd *tmp, int prev_fd, t_ms *ms)
{
    char *cmd;

    close_cmds(tmp->next);
    if (tmp->args[0][0] == '/' || (tmp->args[0][0] == '.' && tmp->args[0][1] == '/'))
        cmd = ft_strdup(tmp->args[0]);
    else
        cmd = get_cmd_path(tmp->args[0], ms);
    if (cmd)
        execve(cmd, tmp->args, ms->envp_cpy);
    perror("execve");
    clean_child(tmp, prev_fd, ms);
    exit(127);
}

void it_is_not_ok(t_cmd *tmp, int prev_fd, int fd[2], t_ms *ms)
{
    in_out_cmds(tmp, prev_fd, fd);
    clean_child(tmp, prev_fd, ms);
    exit(1);
}

void it_is_okay(t_cmd *tmp, int prev_fd, int fd[2], t_ms *ms)
{
    in_out_cmds(tmp, prev_fd, fd);
    signal(SIGINT, SIG_DFL);
    signal(SIGQUIT, SIG_DFL);
    if ((!tmp || !tmp->args || !tmp->args[0]))
    {
        clean_child(tmp, prev_fd, ms);
        exit(1);
    }
    else if (is_builtin(tmp->args[0]))
    {
        int e;
        execute_builtin(tmp->args, ms);
        e = ms->last_exit_status;
        clean_child(tmp, prev_fd, ms);
        exit(e);
    }
    else
        child_execve(tmp, prev_fd, ms);
}

void dad_thing(t_cmd *tmp, int *prev_fd, int fd[2])
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

void fail_pipe(t_ms *ms, t_cmd *tmp, int prev_fd)
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

void fail_fork(t_ms *ms, t_cmd *tmp, int prev_fd, int fd[2])
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

void exec_cmd(t_ms *ms)
{
    t_cmd *tmp;
    int fd[2];
    int prev_fd;
    pid_t pid;

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

