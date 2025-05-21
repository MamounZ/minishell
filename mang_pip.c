/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mang_pip.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaman-alrifai <yaman-alrifai@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/28 20:53:29 by yaman-alrif       #+#    #+#             */
/*   Updated: 2025/05/21 20:33:32 by yaman-alrif      ###   ########.fr       */
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
				{
					ft_strncat(expanded, &value[0], ft_strlen(value));
				}
				ft_memset(var_name, 0, sizeof(var_name));
				//free(value);
			}
			else
			{
				// If "$" is alone in quotes, preserve it (echo "$")
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
		// fprintf(stderr, "expanded: %s\n", expanded);
	}
	return (expanded);
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
            if (tmp->next->value[0] == '"' || tmp->next->value[0] == '\'')
            {
                rm_quote_c(tmp->next->value);
                expand = 0;
            }
            if (pipe(pipe_fd) == -1)
            {
                perror("pipe");
                break;
            }
            char *line;
            while (1)
            {
                line = readline("> ");
                if (!line)
                {
                    perror("getline");
                    free(line);
                    close(pipe_fd[0]);
                    close(pipe_fd[1]);
                    break;
                }
                char *new;
                if (expand)
                    new = expand_heredoc(NULL, line, ms);
                else
                    new = ft_strdup(line);
                if (strcmp(line, tmp->next->value) == 0)
                {
                    free(new);
                    break;
                }
                write(pipe_fd[1], new, ft_strlen(new));
                write(pipe_fd[1], "\n", 1);
                free(new);
            }
            free(line);
            close(pipe_fd[1]);
            add_heredoc(&ms->doc, pipe_fd[0]);
        }
        tmp = tmp->next;
    }
}

void fill_cmds_file(t_ms *ms)
{
    fill_here_doc(ms);
    t_token *tmp = ms->tokens;
    t_cmd *cmd = create_cmd(tmp);
    t_token *tm = NULL;
    t_heredoc *t = ms->doc;

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
            cmd->fd_in = t->fd;
            t = t->n;
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

void fill_cmds(t_cmd *cmd, t_token *tm, t_ms *ms)
{
    t_token *tmp, *tmo;
    int i = 0;
    char *input = tokenize_to_char(tm);
    char *expanded_input = expand_variables(NULL, input, ms);
    // fprintf(stderr, "expanded_input: %s\n", expanded_input);
    tmp = tokenize(expanded_input);
    tmo = tmp;
    free(input);
    free(expanded_input);
    // print_tokens(tmp);
    rm_quote(tmp);

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
    cmd->args[i] = NULL;
    free_tokens(tmo);
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

    if (tmp && !tmp->next && tmp->args && tmp->args[0] && (!ft_strcmp(tmp->args[0], "cd") || !ft_strcmp(tmp->args[0], "export") ||
    !ft_strcmp(tmp->args[0], "unset") || !ft_strcmp(tmp->args[0], "exit")) && tmp->it_is_ok)
        execute_builtin(tmp->args, ms);
    else
    {
        while (tmp)
        {
            if (tmp->next && pipe(fd) == -1)
            {
                perror("pipe");
                exit(1);
            }
            pid = fork();
            if (pid == 0 && tmp->it_is_ok == 0)
            {
                if (tmp->next)
                {
                    close(fd[0]);
                    close(fd[1]);
                }
                if (tmp->fd_in != -1)
                    close(tmp->fd_in);
                if (tmp->fd_out != -1)
                    close(tmp->fd_out);
                if (prev_fd != -1)
                    close(prev_fd);
                free_cmds(ms->cmds);
                free_args(ms->envp_cpy);
	            free_tokens(ms->tokens);
                free_doc(ms->doc);
    	        free(ms);
                close(stdin_copy);
                close(stdout_copy);
                exit(1);
            }
            else if (pid == 0)
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
                if (!tmp || !tmp->args || !tmp->args[0])
                {
                    if (tmp->fd_in != -1)
                        close(tmp->fd_in);
                    if (tmp->fd_out != -1)
                        close(tmp->fd_out);
                    if (prev_fd != -1)
                        close(prev_fd);
                    free_cmds(ms->cmds);
                    free_args(ms->envp_cpy);
	                free_tokens(ms->tokens);
                    free_doc(ms->doc);
    	            free(ms);
                    close(stdin_copy);
                    close(stdout_copy);
                    exit(1);
                }
                else if (is_builtin(tmp->args[0]))
                {
                    int e;
                    execute_builtin(tmp->args, ms);
                    if (tmp->fd_in != -1)
                        close(tmp->fd_in);
                    if (tmp->fd_out != -1)
                        close(tmp->fd_out);
                    if (prev_fd != -1)
                        close(prev_fd);
                    free_cmds(ms->cmds);
                    free_args(ms->envp_cpy);
	                free_tokens(ms->tokens);
                    free_doc(ms->doc);
                    e = ms->last_exit_status;
    	            free(ms);
                    close(stdin_copy);
                    close(stdout_copy);
                    exit(e);
                }
                else 
                {               
                    if (tmp->args[0][0] == '/' || (tmp->args[0][0] == '.' && tmp->args[0][1] == '/'))
                        cmd = ft_strdup(tmp->args[0]);
                    else
                        cmd = get_cmd_path(tmp->args[0], ms);
                    if (cmd)
                        execve(cmd, tmp->args, ms->envp_cpy);
                    perror("execve");
                    if (tmp->fd_in != -1)
                        close(tmp->fd_in);
                    if (tmp->fd_out != -1)
                        close(tmp->fd_out);
                    if (prev_fd != -1)
                        close(prev_fd);
                    free_cmds(ms->cmds);
                	free_args(ms->envp_cpy);
	                free_tokens(ms->tokens);
                    free_doc(ms->doc);
    	            free(ms);
                    close(stdin_copy);
                    close(stdout_copy);
                    exit(127);
                }
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
