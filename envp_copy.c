/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   envp_copy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaman-alrifai <yaman-alrifai@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 18:13:34 by mazaid            #+#    #+#             */
/*   Updated: 2025/05/30 09:20:24 by yaman-alrif      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	count_env_vars(char	**envp)
{
	int	count;

	count = 0;
	while (envp[count])
		count++;
	return (count);
}

void	copy_env(char **envp, t_ms *ms)
{
	int	i;
	int	count;

	count = count_env_vars(envp);
	ms->envp_cpy = malloc((count + 1) * sizeof(char *));
	if (!ms->envp_cpy)
	{
		free(ms);
		exit (1);
	}
	i = 0;
	while (i < count)
	{
		ms->envp_cpy[i] = ft_strdup(envp[i]);
		i++;
	}
	ms->envp_cpy[i] = NULL;
}
