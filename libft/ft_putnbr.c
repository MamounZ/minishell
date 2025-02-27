/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putnbr.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mazaid <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/08 16:07:52 by mazaid            #+#    #+#             */
/*   Updated: 2024/09/08 16:08:12 by mazaid           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int	ft_putnbr(long int nb)
{
	int	count;

	count = 0;
	if (nb < 0)
	{
		count += ft_putchar('-');
		nb = -nb;
	}
	if (nb >= 10)
	{
		count += ft_putnbr(nb / 10);
		count += ft_putnbr(nb % 10);
	}
	else
		count += ft_putchar(nb + '0');
	return (count);
}
