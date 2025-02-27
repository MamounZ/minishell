/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   envp_copy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mazaid <mazaid@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 18:13:34 by mazaid            #+#    #+#             */
/*   Updated: 2025/02/26 19:09:31 by mazaid           ###   ########.fr       */
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
		return ;
	i = 0;
	while (i < count)
	{
		ms->envp_cpy[i] = ft_strd(envp[i]);
		i++;
	}
	ms->envp_cpy[i] = NULL;
}
