/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mang_pip_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mazaid <mazaid@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/05 16:21:35 by mazaid            #+#    #+#             */
/*   Updated: 2025/06/05 16:50:59 by mazaid           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	len_handle_special_dollar_cases_here(char *input, t_expand *e, t_ms *ms)
{
	if (input[e->i] == '0')
	{
		e->size += ft_strlen(ms->argv[0]);
		e->i++;
		return (1);
	}
	else if (input[e->i] >= '1' && input[e->i] <= '9')
	{
		e->i++;
		return (1);
	}
	else if (input[e->i] == '?')
	{
		e->size += ft_numlen(ms->last_exit_status);
		e->i++;
		return (1);
	}
	return (0);
}

int	len_handle_variable_expansion_heredoc(char *input, t_expand *e, t_ms *ms)
{
	if (is_valid_var_char(input[e->i]))
	{
		e->j = 0;
		while (is_valid_var_char(input[e->i]))
			e->var_name[e->j++] = input[e->i++];
		e->var_name[e->j] = '\0';
		e->value = ft_getenv(e->var_name, ms);
		if (e->value)
			e->size += ft_strlen(e->value);
		ft_memset(e->var_name, 0, sizeof(e->var_name));
		return (1);
	}
	return (0);
}

void	len_handle_plain_dollar(char *input, t_expand *e)
{
	e->size++;
	if (input[e->i])
	{
		e->size++;
		e->i++;
	}
}

int	len_heredoc_exp(t_expand *e, char *input, t_ms *ms)
{
	while (input[e->i])
	{
		if (input[e->i] == '$' && input[e->i + 1])
		{
			e->i++;
			if (len_handle_special_dollar_cases_here(input, e, ms))
				continue ;
			if (len_handle_variable_expansion_heredoc(input, e, ms))
				continue ;
			len_handle_plain_dollar(input, e);
		}
		else
		{
			e->size++;
			e->i++;
		}
	}
	e->i = 0;
	return (e->size);
}

void	add_heredoc(t_ms *ms, int fd)
{
	t_heredoc	*new;
	t_heredoc	*tmp;

	new = malloc(sizeof(t_heredoc));
	if (!new)
	{
		ft_free_ms(ms, 1);
		exit(1);
	}
	tmp = ms->doc;
	new->fd = fd;
	new->n = NULL;
	if (!(tmp))
	{
		ms->doc = new;
		return ;
	}
	while (tmp->n)
	{
		tmp = tmp->n;
	}
	tmp->n = new;
}
