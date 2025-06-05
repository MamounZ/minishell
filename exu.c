/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exu.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaman-alrifai <yaman-alrifai@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 15:26:55 by yaman-alrif       #+#    #+#             */
/*   Updated: 2025/06/05 11:29:28 by yaman-alrif      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"


int it_is_this(char *tmp, char *path, char **paths)
{
    if (tmp)
        free(tmp);
    if (!access(path, F_OK))
    {
        free_args(paths);
        return (1);
    }
    if (path)
        free(path);
    return (0);
}
char *get_cmd_path(char *cmd, t_ms *ms)
{
    char *path;
    char *tmp;
    char **paths;
    int i;
    
    paths = ft_split(ft_getenv("PATH", ms), ':');
    if (!paths || !paths[0])
    {
        ft_putstr_fd("minishell: PATH not set or meme erorre\n", STDERR_FILENO);
        return (NULL);
    }
    i = -1;
    while (paths[++i])
    {
        tmp = ft_strjoin(paths[i], "/");
        path = ft_strjoin(tmp, cmd);
        if (it_is_this(tmp, path, paths))
            return (path);
    }
    free_args(paths);
    return (NULL);
}

