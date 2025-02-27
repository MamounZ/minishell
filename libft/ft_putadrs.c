/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putadrs.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mazaid <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/08 16:09:43 by mazaid            #+#    #+#             */
/*   Updated: 2024/09/08 16:10:00 by mazaid           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int	ft_putadrs(size_t value)
{
	int	count;

	count = 0;
	if (value == 0)
		count += ft_putstr("(nil)");
	else
	{
		count += ft_putchar('0');
		count += ft_putchar('x');
		count += ft_puthex(value, 'x');
	}
	return (count);
}
