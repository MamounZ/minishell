/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mazaid <mazaid@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/05 01:11:18 by mazaid            #+#    #+#             */
/*   Updated: 2025/06/05 14:55:02 by mazaid           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	remove_env_var(char *var, t_ms *ms)
{
	int		i;
	int		j;
	size_t	var_len;

	i = 0;
	j = 0;
	var_len = ft_strlen(var);
	while (ms->envp_cpy[i])
		i++;
	ms->new_env = malloc(sizeof(char *) * i);
	if (!ms->new_env)
		return ;
	i = -1;
	while (ms->envp_cpy[++i])
	{
		if (ft_strncmp(ms->envp_cpy[i], var, var_len) == 0
			&& ms->envp_cpy[i][var_len] == '=')
			free(ms->envp_cpy[i]);
		else
			ms->new_env[j++] = ms->envp_cpy[i];
	}
	ms->new_env[j] = NULL;
	free(ms->envp_cpy);
	ms->envp_cpy = ms->new_env;
	ms->new_env = NULL;
}

void	ft_unset(char **args, t_ms *ms)
{
	int	i;

	i = 1;
	if (!args[1])
		return ;
	while (args[i])
	{
		remove_env_var(args[i], ms);
		i++;
	}
}
