/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_puthex.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mazaid <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/08 16:08:59 by mazaid            #+#    #+#             */
/*   Updated: 2024/09/08 16:09:18 by mazaid           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int	ft_puthex(size_t nb, char c)
{
	char	result;
	int		count;

	count = 0;
	result = '\0';
	if (nb >= 16)
	{
		count += ft_puthex(nb / 16, c);
		count += ft_puthex(nb % 16, c);
	}
	else
	{
		if (nb < 10)
			result = nb + '0';
		else
		{
			if (c == 'x')
				result = (nb - 10) + 'a';
			else
				result = (nb - 10) + 'A';
		}
		count += ft_putchar(result);
	}
	return (count);
}
