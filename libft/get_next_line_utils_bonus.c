/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils_bonus.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dkhalil <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/13 00:57:31 by mazaid            #+#    #+#             */
/*   Updated: 2024/09/27 01:24:27 by mazaid           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "get_next_line_bonus.h"

char	*newline_exist(char *str)
{
	int	i;

	i = 0;
	if (!str)
		return (NULL);
	while (str[i])
	{
		if (str[i] == '\n')
			return (&str[i]);
		i++;
	}
	return (NULL);
}

size_t	ft_strlen(char *str)
{
	size_t	i;

	i = 0;
	while (str[i])
		i++;
	return (i);
}

char	*ft_strjoin(char *left, char *buff)
{
	int		i;
	int		j;
	char	*temp;

	if (!left && !buff)
		return (NULL);
	if (!left)
		left = ft_calloc(1, sizeof(char));
	temp = malloc(sizeof(char) * (ft_strlen(left) + ft_strlen(buff) + 1));
	if (!temp)
		return (NULL);
	i = -1;
	while (left[++i])
		temp[i] = left[i];
	j = -1;
	while (buff[++j])
		temp[i++] = buff[j];
	temp[i] = '\0';
	return (free(left), free(buff), temp);
}

void	*ft_calloc(size_t nitem, size_t size)
{
	size_t			totalsize;
	size_t			i;
	unsigned char	*ptr;

	totalsize = size * nitem;
	i = 0;
	ptr = (unsigned char *)malloc(totalsize);
	if (!ptr)
		return (NULL);
	while (i < totalsize)
	{
		ptr[i] = 0;
		i++;
	}
	return ((void *)ptr);
}
