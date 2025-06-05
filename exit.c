/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mazaid <mazaid@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/19 14:54:36 by yaman-alrif       #+#    #+#             */
/*   Updated: 2025/06/05 14:31:13 by mazaid           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

long long	ft_atoll(const char *str)
{
	long long	result;
	int			sign;

	result = 0;
	sign = 1;
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

static int	check_long_input(char *str, int j, int i)
{
	if ((str[j] != '-' && i - j > 19) || ((str[j] == '-' && i - j > 20)))
		return (0);
	if ((str[j] != '-' && i - j < 19) || ((str[j] == '-' && i - j < 20)))
		return (1);
	if (str[j] == '-')
	{
		if (ft_strncmp(str + j, "-9223372036854775808", 20))
			return (0);
	}
	else
	{
		if (ft_strncmp(str + j, "9223372036854775807", 19))
			return (0);
	}
	return (1);
}

int	valid_atoll(char *str)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
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
	return (check_long_input(str, j, i));
}

void	ft_exit(char **args, t_ms *ms)
{
	int	e;

	if (args[1] == NULL)
	{
		ft_free_ms(ms, 1);
		exit(0);
	}
	else if (!valid_atoll(args[1]))
	{
		ft_printf("exit: %s: numeric argument required\n", args[1]);
		ft_free_ms(ms, 1);
		exit(2);
	}
	if (args[2] != NULL)
	{
		ft_printf("exit: too many arguments\n");
		ms->last_exit_status = 2;
		return ;
	}
	printf("exit\n");
	e = ft_atoll(args[1]);
	ft_free_ms(ms, 1);
	exit(e);
}
