/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exu.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaman-alrifai <yaman-alrifai@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 15:26:55 by yaman-alrif       #+#    #+#             */
/*   Updated: 2025/06/05 09:22:38 by yaman-alrif      ###   ########.fr       */
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
        if (!access(path, F_OK)) {
            fprintf(stderr, "Command found: %s\n", path);
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
