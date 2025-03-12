/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mazaid <mazaid@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/03 00:42:24 by mazaid            #+#    #+#             */
/*   Updated: 2025/03/04 16:25:13 by mazaid           ###   ########.fr       */
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
char **realloc_env(char **envp, char *new_var)
{
	int i = 0;
	char **new_envp;

	while (envp[i]) // Count existing vars
		i++;

	new_envp = (char **)malloc(sizeof(char *) * (i + 2)); // +1 for new var, +1 for NULL
	if (!new_envp)
		return (NULL);

	i = 0;
	while (envp[i]) // Copy old variables
	{
		new_envp[i] = ft_strdup(envp[i]);
		i++;
	}

	new_envp[i] = ft_strdup(new_var); // Add new variable
	new_envp[i + 1] = NULL;			  // Null-terminate array

	free_env(envp); // Free old array
	return (new_envp);
}

/* Check if a variable name is valid */
int is_valid_identifier(char *str)
{
	int i = 0;

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

/* Validate all arguments before modifying envp_cpy */
int validate_all_args(char **args)
{
	int i = 1;
	char *name;
	char *equal_sign;

	while (args[i])
	{
		equal_sign = ft_strchr(args[i], '=');
		// Handle cases like "export a", "export a="
		if (equal_sign)
			name = ft_substr(args[i], 0, equal_sign - args[i]); // Extract variable name
		else
			name = ft_strdup(args[i]); // Case: "export a"
		ft_printf("name is %sb\n", name);
		// Check if the variable name is valid
		if (!is_valid_identifier(name))
		{
			ft_printf("minishell: export: `%s': not a valid identifier\n", args[i]);
			free(name);
			return (0); // Return failure
		}
		// Handle invalid cases like "export a =mamoun" or "export a= mamoun"
		if (equal_sign)
		{
			// Check if there is an unexpected space before or after '='
			if (equal_sign > args[i] && (*(equal_sign - 1) == ' ' || (equal_sign[1] == ' ')))
			{
				ft_printf("minishell: export: `%s': not a valid assignment\n", args[i]);
				free(name);
				return (0);
			}
		}
		else
		{
			ft_printf("hi\n");
			return (0);
		}
		free(name);
		i++;
	}
	return (1); // Return success
}

/* Add or update a variable in the environment */
void add_to_env(char *arg, t_ms *ms)
{
	int i = 0;
	char *name;
	char *value;

	name = ft_strdup(arg);
	value = ft_strchr(name, '=');

	if (value)
	{
		*value = '\0'; // Separate name from value
		value++;
	}

	while (ms->envp_cpy[i])
	{
		if (ft_strncmp(ms->envp_cpy[i], name, ft_strlen(name)) == 0 &&
			(ms->envp_cpy[i][ft_strlen(name)] == '=' || ms->envp_cpy[i][ft_strlen(name)] == '\0'))
		{
			free(ms->envp_cpy[i]);
			ms->envp_cpy[i] = ft_strdup(arg);
			free(name);
			return;
		}
		i++;
	}

	ms->envp_cpy = realloc_env(ms->envp_cpy, arg);
	free(name);
}

/* Export command implementation */
void ft_export(char **args, t_ms *ms)
{
	int i = 1;

	//ft_printf("first arg is %s\n", args[1]);
	if (!args[1]) // No arguments, print environment
	{
		print_exported_vars(ms->envp_cpy);
		return;
	}

	if (!validate_all_args(args)) // Validate all args before modifying envp_cpy
		return;

	while (args[i])
	{
		add_to_env(args[i], ms);
		i++;
	}
}
