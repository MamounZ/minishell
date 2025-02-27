/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mazaid <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/03 16:35:25 by mazaid            #+#    #+#             */
/*   Updated: 2024/09/03 22:29:54 by mazaid           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_free_split(char **split)
{
	int	i;

	i = 0;
	while (split[i])
	{
		free(split[i]);
		i++;
	}
	free(split);
}

int	ft_countwords(char const *s, char c)
{
	int	i;
	int	count;
	int	in_word;

	i = 0;
	count = 0;
	in_word = 0;
	while (s[i])
	{
		if (s[i] == c)
			in_word = 0;
		else if (in_word == 0)
		{
			in_word = 1;
			count++;
		}
		i++;
	}
	return (count);
}

char	*ft_inmalloc(char const *s, int start, int end, char **new)
{
	char	*inew;
	int		i;

	inew = (char *)malloc(sizeof(char) * (end - start + 1));
	if (!inew)
	{
		ft_free_split(new);
		return (NULL);
	}
	i = 0;
	while (start < end)
		inew[i++] = s[start++];
	inew[i] = '\0';
	return (inew);
}

char	**ft_split(char const *s, char c)
{
	char	**new;
	int		start;
	int		end;
	int		i;
	int		j;

	new = (char **)malloc(sizeof(char *) * (ft_countwords(s, c) + 1));
	if (!new || !s)
		return (NULL);
	i = 0;
	j = 0;
	while (s[i])
	{
		while (s[i] == c && s[i])
			i++;
		if (s[i] == '\0')
			break ;
		start = i;
		while (s[i] != c && s[i] != '\0')
			i++;
		end = i;
		new[j++] = ft_inmalloc(s, start, end, new);
	}
	new[j] = NULL;
	return (new);
}
