/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mazaid <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/08 16:14:51 by mazaid            #+#    #+#             */
/*   Updated: 2024/09/08 16:15:31 by mazaid           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_PRINTF_H
# define FT_PRINTF_H

# include <unistd.h>
# include <stdarg.h>

int	ft_printf(const char *str, ...);
int	ft_putadrs(size_t value);
int	ft_putchar(char c);
int	ft_puthex(size_t nb, char c);
int	ft_putnbr(long int nb);
int	ft_putstr(char *s);
int	ft_putunbr(unsigned int nb);

#endif
