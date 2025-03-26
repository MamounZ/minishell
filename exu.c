/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exu.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaman-alrifai <yaman-alrifai@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 15:26:55 by yaman-alrif       #+#    #+#             */
/*   Updated: 2025/03/26 07:54:15 by yaman-alrif      ###   ########.fr       */
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
            pid_t pid;
            pid = fork();
            if (pid == 0)
            {
                char **args = ft_split(cmd, ' ');
                cmd = get_cmd_path(args[0], ms);
                // printf("cmd: %s\n", cmd);
                execve(cmd, args, ms->envp_cpy);
                perror("execve");
                free_args(args);
                exit(0);
            }
            else
            {
                wait(NULL);
                free(cmd);
                cmd = ft_strdup("");
            }
        }
        else if (tmp->type == WORD)
        {
            // printf("word: %s\n", tmp->value);
            cmd = ft_strjoin(cmd, " ");
            path = ft_strjoin(cmd, tmp->value);
            free(cmd);
            cmd = path;
        }
        tmp = tmp->next;
    }
    
}



