/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mazaid <mazaid@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/05 01:11:18 by mazaid            #+#    #+#             */
/*   Updated: 2025/03/05 01:26:04 by mazaid           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void remove_env_var(char *var, t_ms *ms)
{
	int i;
	int j;
	char **new_env;
	size_t var_len;

	i = 0;
	j = 0;
	var_len = ft_strlen(var);
	while (ms->envp_cpy[i])
		i++;
	new_env = malloc(sizeof(char *) * i); // Allocate for new env (-1 entry)
	if (!new_env)
		return;
	i = 0;
	while (ms->envp_cpy[i])
	{
		// Check if the current variable starts with var name and is followed by '='
		if (ft_strncmp(ms->envp_cpy[i], var, var_len) == 0 &&
			ms->envp_cpy[i][var_len] == '=')
			free(ms->envp_cpy[i]); // Free the matching variable
		else
			new_env[j++] = ms->envp_cpy[i]; // Copy non-matching entries
		i++;
	}
	new_env[j] = NULL; // NULL-terminate
	free(ms->envp_cpy);
	ms->envp_cpy = new_env; // Assign updated env
}

void ft_unset(char **args, t_ms *ms)
{
	int i;

	i = 1;
	if (!args[1]) // No arguments, do nothing
		return;
	while (args[i])
	{
			remove_env_var(args[i], ms);
		i++;
	}
}
