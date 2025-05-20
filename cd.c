/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaman-alrifai <yaman-alrifai@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/05 22:06:07 by mazaid            #+#    #+#             */
/*   Updated: 2025/05/19 22:16:23 by yaman-alrif      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char *ft_strjoin_free(char *s1, char *s2)
{
	char *joined_str;

	if (!s1 || !s2)
		return (NULL);
	joined_str = ft_strjoin(s1, s2); // Standard string join function
	free(s1); // Free the first string
	return (joined_str);
}

void set_env_value(char *var, char *value, t_ms *ms)
{
	int i = 0;
	char *new_var;
	int len = ft_strlen(var);

	// Construct "VAR=VALUE" string
	new_var = ft_strjoin(var, "=");
	new_var = ft_strjoin_free(new_var, value); // Append value (freeing old memory)

	// Search for existing variable in envp_cpy
	while (ms->envp_cpy[i])
	{
		if (ft_strncmp(ms->envp_cpy[i], var, len) == 0 && ms->envp_cpy[i][len] == '=')
		{
			free(ms->envp_cpy[i]);
			ms->envp_cpy[i] = new_var; // Replace existing variable
			return;
		}
		i++;
	}

	// If variable doesn't exist, add it
	ms->envp_cpy = realloc_env(ms->envp_cpy, new_var);
	free(new_var);
}

void ft_cd(char **args, t_ms *ms)
{
	char *dir;
	char *oldpwd;
	char *newpwd;

	if (args[1] && args[2])
	{
		printf("minishell: cd: too many arguments\n");
		ms->last_exit_status = 1;
		return;
	}

	oldpwd = getcwd(NULL, 0);
	if (!args[1] || strcmp(args[1], "~") == 0) // Default to HOME
	{
		dir = ft_getenv("HOME", ms);
		if (!dir)
		{
			printf("minishell: cd: HOME not set\n");
			ms->last_exit_status = 1;
			free(oldpwd);
			return;
		}
	}
	else if (strcmp(args[1], "-") == 0) // Go to OLDPWD
	{
		dir = ft_getenv("OLDPWD", ms);
		ms->last_exit_status = 0;
		if (dir)
			printf("%s\n", dir); // Print the new path when using `cd -`
		else
		{
			ms->last_exit_status = 1;
			printf("minishell: cd: OLDPWD not set\n");
			free(oldpwd);
			return;
		}
	}
	else
		dir = args[1];

	if (chdir(dir) != 0)
	{
		perror("minishell: cd");
		free(oldpwd);
		return;
	}
	newpwd = getcwd(NULL, 0);
	// Update PWD and OLDPWD in env
	set_env_value("OLDPWD", oldpwd, ms);
	set_env_value("PWD", newpwd, ms);
	free(oldpwd);
}
