/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dkhalil <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/13 00:42:42 by mazaid            #+#    #+#             */
/*   Updated: 2024/10/08 02:52:32 by mazaid           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "get_next_line_bonus.h"

char	*read_file(int fd, char *left)
{
	char	*buff;
	ssize_t	readed;

	readed = 1;
	while (!newline_exist(left) && readed != 0)
	{
		buff = malloc(sizeof(char) * (BUFFER_SIZE + 1));
		if (!buff)
			return (NULL);
		readed = read(fd, buff, BUFFER_SIZE);
		if ((!left && readed == 0) || readed == -1)
			return (free(buff), NULL);
		buff[readed] = '\0';
		left = ft_strjoin(left, buff);
	}
	return (left);
}

char	*ft_get_line(char *left)
{
	char	*line;
	int		i;

	if (!left)
		return (NULL);
	i = 0;
	while (left[i] && left[i] != '\n')
		i++;
	line = malloc(sizeof(char) * (i + 2));
	if (!line)
		return (NULL);
	i = -1;
	while (left[++i] && left[i] != '\n')
		line[i] = left[i];
	if (left[i] == '\n')
	{
		line[i] = '\n';
		i++;
	}
	line[i] = '\0';
	return (line);
}

char	*ft_get_rest(char *left, char *line)
{
	char	*rest;
	int		i;
	int		j;

	i = ft_strlen(line);
	if (!left[i])
		return (free(left), NULL);
	j = 0;
	while (left[i++])
		j++;
	rest = malloc(sizeof(char) * (j + 1));
	if (!rest)
		return (NULL);
	i = ft_strlen(line);
	j = 0;
	while (left[i])
		rest[j++] = left[i++];
	rest[j] = '\0';
	return (free(left), rest);
}

char	*get_next_line(int fd)
{
	static char	*left[1024];
	char		*line;
	char		*temp;

	if (fd < 0 || BUFFER_SIZE <= 0 || fd > 1024)
		return (NULL);
	temp = read_file(fd, left[fd]);
	if (!temp)
	{
		if (left[fd])
			free(left[fd]);
		left[fd] = NULL;
		return (NULL);
	}
	left[fd] = temp;
	line = ft_get_line(left[fd]);
	left[fd] = ft_get_rest(left[fd], line);
	return (line);
}
