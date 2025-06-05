/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exu_2.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mazaid <mazaid@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 19:05:25 by yaman-alrif       #+#    #+#             */
/*   Updated: 2025/06/05 16:07:40 by mazaid           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*tokenize_to_char(t_token *tokens)
{
	char	*str;
	int		len;
	t_token	*tmp;

	len = 0;
	tmp = tokens;
	while (tmp)
	{
		len += ft_strlen(tmp->value) + 1;
		tmp = tmp->next;
	}
	str = malloc(sizeof(char) * (len + 1));
	str[0] = '\0';
	tmp = tokens;
	while (tmp)
	{
		ft_strcat(str, tmp->value);
		ft_strcat(str, " ");
		tmp = tmp->next;
	}
	if (len > 0)
		str[len - 1] = '\0';
	return (str);
}
