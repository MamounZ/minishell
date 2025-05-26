/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaman-alrifai <yaman-alrifai@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/19 14:54:36 by yaman-alrif       #+#    #+#             */
/*   Updated: 2025/05/21 08:23:40 by yaman-alrif      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

long long ft_atoll(const char *str)
{
    long long result = 0;
    int sign = 1;

    while (*str == ' ' || (*str >= 9 && *str <= 13))
        str++;
    if (*str == '-' || *str == '+')
    {
        if (*str == '-')
            sign = -1;
        str++;
    }
    while (*str >= '0' && *str <= '9')
    {
        result = result * 10 + (*str - '0');
        str++;
    }
    return (result * sign);
}

int valid_atoll(char *str)
{
    int i = 0;
    int j = 0;

    if (!str)
        return (0);
    while (str[i] == ' ' || (str[i] >= 9 && str[i] <= 13))
        i++;
    if (str[i] == '\0')
        return (0);
    j = i;
    if (str[i] == '-' || str[i] == '+')
        i++;
    if (str[i] == '\0')
        return (0);
    while (str[i])
    {
        if (str[i] < '0' || str[i] > '9')
            return (0);
        i++;
    }
    // ft_printf("i = %d\n", i);
    // ft_printf("str[%d] = %c\n", i, str[i]);
    if (i > 19)
    {
        if (str[j] == '-')
        {
            if (ft_strncmp(str + j, "-9223372036854775808", 20) > 0)
                return (0);
        }
        else
        {
            if (ft_strncmp(str + j, "9223372036854775807", 19) > 0)
                return (0);
        }
    }
    return (1);
}

void	ft_exit(char **args, t_ms *ms)
{
    (void)ms;
    if (args[1] == NULL)
        exit(0);
    else if (!valid_atoll(args[1]))
    {
        ft_printf("exit: %s: numeric argument required\n", args[1]);
        exit(2);
    }
    if (args[2] != NULL)
    {
        ft_printf("exit: too many arguments\n");
        ms->last_exit_status = 2;
        return ;
    }
    printf("exit\n");
    exit(ft_atoll(args[1]));
}
