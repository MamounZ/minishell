/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mazaid <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/05 15:30:19 by mazaid            #+#    #+#             */
/*   Updated: 2024/09/05 16:10:11 by mazaid           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"
//#include <stdio.h>

static void	flags(char str, va_list	args, int *count)
{
	if (str == 'c')
		*count += ft_putchar(va_arg(args, int));
	else if (str == 's')
		*count += ft_putstr(va_arg(args, char *));
	else if (str == 'd' || str == 'i')
		*count += ft_putnbr(va_arg(args, int));
	else if (str == 'u')
		*count += ft_putunbr(va_arg(args, unsigned int));
	else if (str == '%')
		*count += ft_putchar('%');
	else if (str == 'x')
		*count += ft_puthex(va_arg(args, unsigned int), str);
	else if (str == 'X')
		*count += ft_puthex(va_arg(args, unsigned int), str);
	else if (str == 'p')
		*count += ft_putadrs(va_arg(args, size_t));
	else
	{
		*count += ft_putchar('%');
		*count += ft_putchar(str);
	}
}

int	ft_printf(const char *str, ...)
{
	va_list	args;
	int		count;

	va_start (args, str);
	count = 0;
	while (*str)
	{
		if (*str == '%' && *(str + 1))
		{
			str++;
			flags(*str, args, &count);
		}
		else if (*str == '%' && !(*(str + 1)))
			return (-1);
		else
			count += ft_putchar(*str);
		str++;
	}
	va_end(args);
	return (count);
}
/*
int	main()
{
	int	*ptr;
	printf("%d\n",ft_printf("mine  %x\n", -1));
	printf("%d\n",printf("orgnl %x\n", -1));
	printf("%d\n",ft_printf("mine  %u\n", -1));
	printf("%d\n",printf("orgnl %u\n", -1));
	printf("%d\n",ft_printf("mine  %c\n", 'c'));
	printf("%d\n",printf("orgnl %c\n", 'c'));
	printf("%d\n",ft_printf("mine  %s\n", "mamoun zaid"));
	printf("%d\n",printf("orgnl %s\n", "mamoun zaid"));
	printf("%d\n",ft_printf("uuuumine  %u\n", -5));
	printf("%d\n",printf("uuuuorgnl %u\n", -5));
	printf("%d\n",ft_printf("mine  %%\n", "mamoun zadi"));
	printf("%d\n",printf("orgnl %%\n", "mamoun zaid"));
	printf("%d\n",ft_printf("mine  %x\n", 14259));
	printf("%d\n",printf("orgnl %x\n", 14259));
	printf("%d\n",ft_printf("mine  %X\n", 14259));
	printf("%d\n",printf("orgnl %X\n", 14259));
	printf("%d\n",ft_printf("mine  %p\n", &ptr));
	printf("%d\n",printf("orgnl %p\n", &ptr));
	printf("%d", printf("orgl mamoun %"));
	printf("\n");
	printf("%d", ft_printf("mine mamoun %"));
	printf("\n");
	return (0);
}*/
