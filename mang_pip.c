/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mang_pip.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaman-alrifai <yaman-alrifai@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/28 20:53:29 by yaman-alrif       #+#    #+#             */
/*   Updated: 2025/05/30 09:57:37 by yaman-alrif      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void add_heredoc(t_heredoc **head, int fd)
{
    t_heredoc *new = malloc(sizeof(t_heredoc));
    t_heredoc *tmp = *head;

    new->fd = fd;
    new->n = NULL;
    if (!(*head))
    {
        *head = new;
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

char	*expand_heredoc(char **argv, char *input, t_ms *ms)
{
	int i;
	int j;
	char *expanded;
	char var_name[256];
	char *exit_status;
	char *value;

	expanded = malloc(10000);
	if (!expanded)
		return NULL;
	expanded[0] = '\0';
	i = 0;
	while (input[i])
	{
		if (input[i] == '$' && input[i + 1])
		{
			i++;
			if (input[i] == '0')
			{
				ft_strcat(expanded, argv[0]);
				i++;
			}
			else if (input[i] >= '1' && input[i] <= '9')
			{
				ft_strcat(expanded, "");
				i++;
			}
			else if (input[i] == '?')
			{
				exit_status = ft_itoa(ms->last_exit_status);
				ft_strcat(expanded, exit_status);
				free(exit_status);
				i++;
			}
			else if (is_valid_var_char(input[i]))
			{
				j = 0;
				while (is_valid_var_char(input[i]))
					var_name[j++] = input[i++];
				var_name[j] = '\0';
				value = ft_getenv(var_name, ms);
				if (value)
					ft_strncat(expanded, &value[0], ft_strlen(value));
				ft_memset(var_name, 0, sizeof(var_name));
			}
			else
			{
                ft_strncat(expanded, "$", 1);
				ft_strncat(expanded, &input[i], 1);
                i++;
			}
		}
		else
		{
			ft_strncat(expanded, &input[i], 1);
			i++;
		}
	}
	return (expanded);
}

int heredoc_loop(t_token *tmp, int pipe_fd[2], int expand, t_ms *ms)
{
    char    *line;
    char    *new;

    line = readline("> ");
    if (!line && close(pipe_fd[0]) && close(pipe_fd[1]))
    {
        perror("getline");
        free(line);
        return 0;
    }
    if (expand)
        new = expand_heredoc(NULL, line, ms);
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
                break ;
            }
            while (heredoc_loop(tmp, pipe_fd, expand, ms))
                ;
            close(pipe_fd[1]);
            add_heredoc(&ms->doc, pipe_fd[0]);
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
        *cmd = create_cmd(tmp->next);
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
    tmp = ms->tokens;
    tm = NULL;
    t = ms->doc;
    cmd = create_cmd(tmp);
    while (tmp)
    {
        if (tmp->type == REDIR_OUT || tmp->type == REDIR_IN || tmp->type == APPEND || tmp->type == HEREDOC)
        {
            find_in_or_out(tmp, cmd, &t);
            tmp = tmp->next;
        }
        else if (tmp->type != PIPE)
            add_token(&tm, new_token(ft_strdup(tmp->value), tmp->type));
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

void fill_cmds(t_cmd *cmd, t_token *tm, t_ms *ms)
{
    t_token *tmp;
    t_token *tmo;
    int i;
    char *input;
    char *expanded_input;

    i = 0;
    input = tokenize_to_char(tm);
    expanded_input = expand_variables(NULL, input, ms);
    tmp = tokenize(expanded_input);
    tmo = tmp;
    free(input);
    free(expanded_input);
    rm_quote(tmp);
    cmd->args = malloc(sizeof(char *) * (token_size(tmp) + 1));
    while (tmp)
    {
        cmd->args[i++] = dup_token(tmp);
        tmp = tmp->next;
    }
    cmd->args[i] = NULL;
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
        {
            perror("pipe");
            exit(1);
        }
        signal(SIGINT, SIG_IGN);
        pid = fork();
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

