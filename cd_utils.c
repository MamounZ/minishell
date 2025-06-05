/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_utils.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mazaid <mazaid@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/05 14:22:48 by mazaid            #+#    #+#             */
/*   Updated: 2025/06/05 14:27:39 by mazaid           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*ft_strjoin_free(char *s1, char *s2)
{
	char	*joined_str;

	if (!s1 || !s2)
		return (NULL);
	joined_str = ft_strjoin(s1, s2);
	free(s1);
	return (joined_str);
}

void	set_env_value(char *var, char *value, t_ms *ms)
{
	int		i;
	int		len;
	char	*new_var;

	i = 0;
	len = ft_strlen(var);
	new_var = ft_strjoin(var, "=");
	new_var = ft_strjoin_free(new_var, value);
	while (ms->envp_cpy[i])
	{
		if (ft_strncmp(ms->envp_cpy[i], var, len) == 0
			&& ms->envp_cpy[i][len] == '=')
		{
			free(ms->envp_cpy[i]);
			ms->envp_cpy[i] = new_var;
			return ;
		}
		i++;
	}
	ms->envp_cpy = realloc_env(ms->envp_cpy, new_var);
	free(new_var);
}
