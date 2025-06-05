/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_len.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mazaid <mazaid@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/05 14:33:06 by mazaid            #+#    #+#             */
/*   Updated: 2025/06/05 14:50:50 by mazaid           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	len_copy_value(t_expand *e)
{
	int	z;

	z = 0;
	while (e->value[z])
	{
		if (e->value[z] == '\"' && !e->in_double_quotes)
			e->size += 3;
		else if (e->value[z] == '\"' && e->in_double_quotes)
			e->size += 5;
		else if (e->value[z] == '\'' && !e->in_double_quotes)
			e->size += 3;
		else
			e->size += 1;
		z++;
	}
}

int	len_handle_special_dollar_cases(char *input, t_expand *e, t_ms *ms)
{
	char	*exit_status;

	if (input[e->i] == '0')
	{
		e->i++;
		(e->size)++;
		return (1);
	}
	else if (input[e->i] >= '1' && input[e->i] <= '9')
	{
		e->i++;
		return (1);
	}
	else if (input[e->i] == '?')
	{
		exit_status = ft_itoa(ms->last_exit_status);
		e->size += ft_strlen(exit_status); // yaman -> do ft_numlen
		free(exit_status);
		e->i++;
		return (1);
	}
	return (0);
}

int	len_handle_variable_expansion(char *input, t_expand *e, t_ms *ms)
{
	if (is_valid_var_char(input[e->i]))
	{
		e->j = 0;
		while (is_valid_var_char(input[e->i]))
			e->var_name[e->j++] = input[e->i++];
		e->var_name[e->j] = '\0';
		e->value = ft_getenv(e->var_name, ms);
		if (e->value)
			len_copy_value(e);
		ft_memset(e->var_name, 0, sizeof(e->var_name));
		return (1);
	}
	return (0);
}

int	len_handle_unknown_dollar(char *input, t_expand *e)
{
	int	size;

	size = 0;
	if (e->in_double_quotes && (input[e->i] == '\'' || input[e->i] == '\"'))
	{
		return (1);
	}
	if (input[e->i] == '\'' || input[e->i] == '\"')
		return (0);
	size++;
	if (input[e->i])
	{
		size++;
		e->i++;
	}
	return (size);
}

int	ft_lenexpand(char *input, t_ms *ms)
{
	t_expand	e;

	ft_bzero(&e, sizeof(e));
	while (input[e.i])
	{
		if (len_handle_quotes(input, &e))
			e.size++;
		if (input[e.i] == '$' && input[e.i + 1] && !e.in_single_quotes)
		{
			e.i++;
			if (len_handle_special_dollar_cases(input, &e, ms))
				continue ;
			if (len_handle_variable_expansion(input, &e, ms))
				continue ;
			e.size += len_handle_unknown_dollar(input, &e);
			continue ;
		}
		else
		{
			e.size++;
			e.i++;
		}
	}
	return (e.size);
}
