/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putunbr.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mazaid <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/08 16:08:28 by mazaid            #+#    #+#             */
/*   Updated: 2024/09/08 16:08:47 by mazaid           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int	ft_putunbr(unsigned int nb)
{
	int	count;

	count = 0;
	if (nb >= 10)
	{
		count += ft_putunbr((nb / 10));
		count += ft_putunbr((nb % 10));
	}
	else
		count += ft_putchar(nb + '0');
	return (count);
}
