/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mazaid <mazaid@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/03 00:42:24 by mazaid            #+#    #+#             */
/*   Updated: 2025/06/04 16:56:47 by mazaid           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* Free the environment array */
void free_env(char **envp)
{
	int i = 0;

	while (envp[i])
	{
		free(envp[i]);
		i++;
	}
	free(envp);
}

/* Expand environment array to add a new variable */

char **realloc_env(char **env, char *new_entry)
{
	int i = 0;
	int j = 0;
	char **new_env;

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

// char **realloc_env(char **envp, char *new_var)
// {
// 	int i = 0;
// 	char **new_envp;

// 	while (envp[i]) // Count existing vars
// 		i++;

// 	new_envp = (char **)malloc(sizeof(char *) * (i + 2)); // +1 for new var, +1 for NULL
// 	if (!new_envp)
// 		return (NULL);

// 	i = 0;
// 	while (envp[i]) // Copy old variables
// 	{
// 		new_envp[i] = ft_strdup(envp[i]);
// 		i++;
// 	}

// 	new_envp[i] = ft_strdup(new_var); // Add new variable
// 	new_envp[i + 1] = NULL;			  // Null-terminate array

// 	free_env(envp); // Free old array
// 	return (new_envp);
// }

/* Check if a variable name is valid */
int is_valid_identifier(char *str)
{
	int i = 1;

	if (!str || (!ft_isalpha(str[0]) && str[0] != '_')) // Must start with a letter or _
		return (0);
	while (str[i])
	{
		if (!ft_isalnum(str[i]) && str[i] != '_') // Only letters, digits, or _
			return (0);
		i++;
	}
	return (1);
}

/* Print exported environment variables */
void print_exported_vars(char **envp)
{
	int i = 0;
	char *equal_sign;

	while (envp[i])
	{
		equal_sign = ft_strchr(envp[i], '=');
		if (equal_sign)
			ft_printf("declare -x %.*s=\"%s\"\n", (int)(equal_sign - envp[i]), envp[i], equal_sign + 1);
		else
			ft_printf("declare -x %s\n", envp[i]); // Print vars without values correctly
		i++;
	}
}

/* Add or update a variable in the environment */

void update_env(t_ms *ms, char *var, char *value)
{
	int i = 0;
	char *new_entry = ft_strjoin(var, "=");
	char *full_entry = ft_strjoin(new_entry, value ? value : "");
	free(new_entry);

	while (ms->envp_cpy[i])
	{
		if (!ft_strncmp(ms->envp_cpy[i], var, ft_strlen(var)) &&
			ms->envp_cpy[i][ft_strlen(var)] == '=')
		{
			free(ms->envp_cpy[i]); // Free old entry
			ms->envp_cpy[i] = full_entry;
			return;
		}
		i++;
	}

	// Add new variable
	ms->envp_cpy = realloc_env(ms->envp_cpy, full_entry);
}

// void add_to_env(char *arg, t_ms *ms)
// {
// 	int i = 0;
// 	char *name;
// 	char *value;

// 	name = ft_strdup(arg);
// 	value = ft_strchr(name, '=');

// 	if (value)
// 	{
// 		*value = '\0'; // Separate name from value
// 		value++;
// 	}

// 	while (ms->envp_cpy[i])
// 	{
// 		if (ft_strncmp(ms->envp_cpy[i], name, ft_strlen(name)) == 0 &&
// 			(ms->envp_cpy[i][ft_strlen(name)] == '=' || ms->envp_cpy[i][ft_strlen(name)] == '\0'))
// 		{
// 			free(ms->envp_cpy[i]);
// 			ms->envp_cpy[i] = ft_strdup(arg);
// 			free(name);
// 			return;
// 		}
// 		i++;
// 	}

// 	ms->envp_cpy = realloc_env(ms->envp_cpy, arg);
// 	free(name);
// }

static void handle_export_assignment(char *arg, t_ms *ms)
{
	char *equal_sign = ft_strchr(arg, '=');
	char *var = ft_substr(arg, 0, equal_sign - arg);
	char *value = ft_strdup(equal_sign + 1);

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
	// If valid with no '=', do nothing
}

void ft_export(char **args, t_ms *ms)
{
	int i = 1;

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

/* Export command implementation */
// void ft_export(char **args, t_ms *ms)
// {
// 	int i = 1; // Start from the first argument after "export"

// 	if (!args[1]) // No arguments: print all env vars
// 	{
// 		for (int j = 0; ms->envp_cpy[j]; j++)
// 			printf("declare -x %s\n", ms->envp_cpy[j]);
// 		return;
// 	}

// 	while (args[i])
// 	{
// 		char *equal_sign = ft_strchr(args[i], '=');
// 		if (equal_sign) // Case: VAR=value
// 		{
// 			char *var = ft_substr(args[i], 0, equal_sign - args[i]); // Get variable name
// 			char *value = ft_strdup(equal_sign + 1);				 // Get value

// 			if (is_valid_identifier(var))
// 				update_env(ms, var, value);
// 			else
// 			{
// 				ms->last_exit_status = 1;
// 				printf("minishell: export: `%s': not a valid identifier\n", args[i]);
// 			}

// 			free(var);
// 			free(value);
// 		}
// 		else // Case: Only VAR (no = sign)
// 		{
// 			if (!is_valid_identifier(args[i])) // Invalid identifier
// 			{
// 				ms->last_exit_status = 1;
// 				printf("minishell: export: `%s': not a valid identifier\n", args[i]);
// 			}
// 			// If valid, do nothing (as per your method)
// 		}
// 		i++;
// 	}
// }
