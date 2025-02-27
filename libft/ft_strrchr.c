/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mazaid <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/28 19:13:49 by mazaid            #+#    #+#             */
/*   Updated: 2024/08/28 19:31:50 by mazaid           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strrchr(const char *s, int c)
{
	int			slen;
	const char	*ptr;

	slen = ft_strlen(s);
	ptr = s + slen;
	while (ptr >= s)
	{
		if (*ptr == (char)c)
			return ((char *)ptr);
		ptr--;
	}
	return (0);
}
