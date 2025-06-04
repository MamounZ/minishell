/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mazaid <mazaid@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/03 00:42:24 by mazaid            #+#    #+#             */
/*   Updated: 2025/06/04 20:31:13 by mazaid           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void free_env(char **envp)
{
	int i;

	i = 0;
	while (envp[i])
	{
		free(envp[i]);
		i++;
	}
	free(envp);
}

char **realloc_env(char **env, char *new_entry)
{
	int i;
	int j;
	char **new_env;

	i = 0;
	j = 0;
	while (env[i])
		i++;
	new_env = malloc(sizeof(char *) * (i + 2));
	if (!new_env)
		return NULL;
	while (j < i)
	{
		new_env[j] = env[j];
		j++;
	}
	new_env[i] = new_entry;
	new_env[i + 1] = NULL;
	free(env);
	return new_env;
}

int is_valid_identifier(char *str)
{
	int i;

	i = 1;
	if (!str || (!ft_isalpha(str[0]) && str[0] != '_'))
		return (0);
	while (str[i])
	{
		if (!ft_isalnum(str[i]) && str[i] != '_')
			return (0);
		i++;
	}
	return (1);
}

void print_exported_vars(char **envp)
{
	int i;
	char *equal_sign;

	i = 0;
	while (envp[i])
	{
		equal_sign = ft_strchr(envp[i], '=');
		if (equal_sign)
			ft_printf("declare -x %.*s=\"%s\"\n", (int)(equal_sign - envp[i]), envp[i], equal_sign + 1);
		else
			ft_printf("declare -x %s\n", envp[i]);
		i++;
	}
}

void update_env(t_ms *ms, char *var, char *value)
{
	int i;
	char *new_entry;
	char *full_entry;

	i = 0;
	new_entry = ft_strjoin(var, "=");
	full_entry = ft_strjoin(new_entry, value ? value : "");
	free(new_entry);
	while (ms->envp_cpy[i])
	{
		if (!ft_strncmp(ms->envp_cpy[i], var, ft_strlen(var)) &&
			ms->envp_cpy[i][ft_strlen(var)] == '=')
		{
			free(ms->envp_cpy[i]);
			ms->envp_cpy[i] = full_entry;
			return;
		}
		i++;
	}
	ms->envp_cpy = realloc_env(ms->envp_cpy, full_entry);
}

static void handle_export_assignment(char *arg, t_ms *ms)
{
	char *equal_sign;
	char *var;
	char *value;

	equal_sign = ft_strchr(arg, '=');
	var = ft_substr(arg, 0, equal_sign - arg);
	value = ft_strdup(equal_sign + 1);
	if (is_valid_identifier(var))
		update_env(ms, var, value);
	else
	{
		ms->last_exit_status = 1;
		printf("minishell: export: `%s': not a valid identifier\n", arg);
	}
	free(var);
	free(value);
}

static void handle_export_argument(char *arg, t_ms *ms)
{
	if (ft_strchr(arg, '='))
		handle_export_assignment(arg, ms);
	else if (!is_valid_identifier(arg))
	{
		ms->last_exit_status = 1;
		printf("minishell: export: `%s': not a valid identifier\n", arg);
	}
}

void ft_export(char **args, t_ms *ms)
{
	int i;

	i = 1;
	if (!args[1])
	{
		for (int j = 0; ms->envp_cpy[j]; j++)
			printf("declare -x %s\n", ms->envp_cpy[j]);
		return;
	}
	while (args[i])
	{
		handle_export_argument(args[i], ms);
		i++;
	}
}
