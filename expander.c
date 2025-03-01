/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mazaid <mazaid@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 16:44:28 by mazaid            #+#    #+#             */
/*   Updated: 2025/03/01 21:23:51 by mazaid           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_valid_var_char(char c)
{
	if (ft_isalnum(c) || c == '_')
		return (1);
	return (0);
}

char	*get_env_value(char *var, char **envp)
{
	int	i;
	int	len;

	i = 0;
	len = ft_strlen(var);
	while (envp[i])
	{
		if (ft_strncmp(envp[i], var, len) == 0 && envp[i][len] == '=')
			return (ft_strdup(envp[i] + len + 1));
		i++;
	}
	return (ft_strdup(""));
}

char	*expand_variables(char *input, char **envp, int last_exit_status)
{
	int i;
	int j;
	char *expanded;
	char var_name[256];
	char *exit_status;
	char *value;
	int in_single_quotes = 0;
	int in_double_quotes = 0;

	expanded = malloc(10000);
	if (!expanded)
		return NULL;
	expanded[0] = '\0';
	i = 0;
	while (input[i])
	{
		if (input[i] == '\'' && !in_double_quotes)
		{
			in_single_quotes = !in_single_quotes; // Toggle single-quote mode
			ft_strncat(expanded, &input[i], 1);
			i++;
			continue;
		}
		else if (input[i] == '\"' && !in_single_quotes)
		{
			in_double_quotes = !in_double_quotes; // Toggle double-quote mode
			ft_strncat(expanded, &input[i], 1);
			i++;
			continue;
		}

		if (input[i] == '$' && input[i + 1] && !in_single_quotes) // Expand only outside single quotes
		{
			i++;
			if (input[i] == '?')
			{
				exit_status = ft_itoa(last_exit_status);
				ft_strcat(expanded, exit_status);
				free(exit_status);
				i++;
			}
			else
			{
				j = 0;
				while (is_valid_var_char(input[i]))
					var_name[j++] = input[i++];
				var_name[j] = '\0';
				value = get_env_value(var_name, envp);
				if (value)
					ft_strcat(expanded, value);
				free(value);
			}
		}
		else
		{
			ft_strncat(expanded, &input[i], 1);
			i++;
		}
	}
	return (expanded);
}
