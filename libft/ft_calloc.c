/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mazaid <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/30 21:40:17 by mazaid            #+#    #+#             */
/*   Updated: 2024/08/30 21:53:10 by mazaid           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_calloc(size_t nmemb, size_t size)
{
	void	*re;

	re = malloc(nmemb * size);
	if (!re)
		return (0);
	ft_bzero(re, (nmemb * size));
	return (re);
}
