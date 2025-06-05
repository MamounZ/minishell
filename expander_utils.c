/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaman-alrifai <yaman-alrifai@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/05 14:36:48 by mazaid            #+#    #+#             */
/*   Updated: 2025/06/05 15:43:43 by yaman-alrif      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_valid_var_char(char c)
{
	if (ft_isalnum(c) || c == '_')
		return (1);
	return (0);
}

void exit_fail_itoa(t_ms *ms)
{
	close_cmds(ms->cmds);
	ft_free_ms(ms, 1);
	exit (1);
}

char	*ft_getenv(char *var, t_ms *ms)
{
	int	i;
	int	len;

	if (!var || !ms || !ms->envp_cpy)
		return (NULL);
	len = ft_strlen(var);
	i = 0;
	while (ms->envp_cpy[i])
	{
		if (ft_strncmp(ms->envp_cpy[i], var, len) == 0
			&& ms->envp_cpy[i][len] == '=')
			return (ms->envp_cpy[i] + len + 1);
		i++;
	}
	return (NULL);
}

int	len_handle_quotes(char *input, t_expand *e)
{
	if (input[e->i] == '\'' && !e->in_double_quotes)
	{
		e->in_single_quotes = !e->in_single_quotes;
		return (1);
	}
	else if (input[e->i] == '\"' && !e->in_single_quotes)
	{
		e->in_double_quotes = !e->in_double_quotes;
		return (1);
	}
	return (0);
}

int	handle_quotes(char *input, t_expand *e)
{
	if (input[e->i] == '\'' && !e->in_double_quotes)
	{
		e->in_single_quotes = !e->in_single_quotes;
		ft_strncat(e->expanded, &input[e->i++], 1);
		return (1);
	}
	else if (input[e->i] == '\"' && !e->in_single_quotes)
	{
		e->in_double_quotes = !e->in_double_quotes;
		ft_strncat(e->expanded, &input[e->i++], 1);
		return (1);
	}
	return (0);
}
