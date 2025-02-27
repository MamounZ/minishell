/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mazaid <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/02 20:25:28 by mazaid            #+#    #+#             */
/*   Updated: 2024/09/03 14:17:52 by mazaid           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_intlen(int n)
{
	int	c;

	c = 0;
	if (n <= 0)
	{
		c++;
		n = -n;
	}
	while (n != 0)
	{
		n = n / 10;
		c++;
	}
	return (c);
}

char	*ft_itoa(int n)
{
	char	*new;
	long	nb;
	int		len;

	nb = n;
	len = ft_intlen(nb);
	new = (char *)malloc(sizeof(char) * (len + 1));
	if (!new)
		return (NULL);
	new[len--] = '\0';
	if (nb < 0)
	{
		new[0] = '-';
		nb = -nb;
	}
	if (nb == 0)
		new[0] = '0';
	while (nb > 0)
	{
		new[len--] = (char)((nb % 10) + '0');
		nb = nb / 10;
	}
	return (new);
}
