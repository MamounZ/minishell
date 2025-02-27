/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mazaid <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/29 00:18:27 by mazaid            #+#    #+#             */
/*   Updated: 2024/08/29 00:21:59 by mazaid           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_atoi(const char *str)
{
	int	i;
	int	sighn;
	int	result;

	i = 0;
	sighn = 1;
	result = 0;
	while (str[i] == 32 || (str[i] <= 13 && str[i] >= 9))
		i++;
	if (str[i] == '-' || str[i] == '+')
	{
		if (str[i] == '-')
			sighn *= -1;
		i++;
	}
	while (str[i] <= '9' && str[i] >= '0')
	{
		result *= 10;
		result += str[i] - '0';
		i++;
	}
	return (result * sighn);
}
