/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mang_pip_utils2.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mazaid <mazaid@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/05 16:21:35 by mazaid            #+#    #+#             */
/*   Updated: 2025/06/05 16:54:07 by mazaid           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	token_size(t_token *tmp)
{
	int	size;

	size = 0;
	while (tmp)
	{
		size++;
		tmp = tmp->next;
	}
	return (size);
}

void	free_doc(t_heredoc *ms)
{
	t_heredoc	*tmp;

	while (ms)
	{
		tmp = ms;
		ms = ms->n;
		free(tmp);
	}
}

int	rm_quote_c(char *str)
{
	int		i;
	int		j;
	char	quote;

	i = 0;
	j = 0;
	quote = 0;
	while (str[i])
	{
		if ((str[i] == '\'' || str[i] == '\"') && !quote)
			quote = str[i];
		else if (str[i] == quote)
			quote = 0;
		else
			str[j++] = str[i];
		i++;
	}
	str[j] = '\0';
	return (1);
}

int	handle_variable_expansion_heredoc(char *input, t_expand *e, t_ms *ms)
{
	if (is_valid_var_char(input[e->i]))
	{
		e->j = 0;
		while (is_valid_var_char(input[e->i]))
			e->var_name[e->j++] = input[e->i++];
		e->var_name[e->j] = '\0';
		e->value = ft_getenv(e->var_name, ms);
		if (e->value)
			ft_strncat(e->expanded, &e->value[0], ft_strlen(e->value));
		ft_memset(e->var_name, 0, sizeof(e->var_name));
		return (1);
	}
	return (0);
}

void	handle_plain_dollar(char *input, t_expand *e)
{
	ft_strncat(e->expanded, "$", 1);
	if (input[e->i])
		ft_strncat(e->expanded, &input[e->i++], 1);
}
