/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaman-alrifai <yaman-alrifai@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/19 14:54:36 by yaman-alrif       #+#    #+#             */
/*   Updated: 2025/05/19 14:55:17 by yaman-alrif      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	exit_shell(t_ms *ms, char **args)
{
    int		i;
    int		exit_code;

    if (args[1] == NULL)
        exit(0);
    i = 0;
    while (args[1][i])
    {
        if (!ft_isdigit(args[1][i]))
            break ;
        i++;
    }
    if (args[1][i] != '\0')
    {
        ft_putstr_fd("exit\n", 2);
        ft_putstr_fd("minishell: exit: ", 2);
        ft_putstr_fd(args[1], 2);
        ft_putstr_fd(": numeric argument required\n", 2);
        free_all(data);
        exit(255);
    }
    exit_code = ft_atoi(args[1]);
    free_all(data);
    exit(exit_code % 256);
}
