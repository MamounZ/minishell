/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaman-alrifai <yaman-alrifai@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 16:44:28 by mazaid            #+#    #+#             */
/*   Updated: 2025/05/01 12:54:42 by yaman-alrif      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_valid_var_char(char c)
{
	if (ft_isalnum(c) || c == '_')
		return (1);
	return (0);
}

// char	*get_env_value(char *var, t_ms *ms)
// {
// 	int	i;
// 	int	len;

// 	i = 0;
// 	len = ft_strlen(var);
// 	while (ms->envp_cpy[i])
// 	{
// 		if (ft_strncmp(ms->envp_cpy[i], var, len) == 0 && ms->envp_cpy[i][len] == '=')
// 			return (ft_strdup(ms->envp_cpy[i] + len + 1));
// 		i++;
// 	}
// 	return (ft_strdup(""));
// }
char *ft_getenv(char *var, t_ms *ms)
{
	int i;
	int len;

	if (!var || !ms || !ms->envp_cpy)
		return (NULL);
	len = ft_strlen(var);
	i = 0;
	while (ms->envp_cpy[i])
	{
		if (ft_strncmp(ms->envp_cpy[i], var, len) == 0 && ms->envp_cpy[i][len] == '=')
			return (ms->envp_cpy[i] + len + 1);
		i++;
	}
	return (NULL);
}
// handle echo "\$USER"
char	*expand_variables(char **argv, char *input, t_ms *ms, int last_exit_status, int need_quotes)
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
			in_single_quotes = !in_single_quotes;
			if (need_quotes == -1)
				ft_strncat(expanded, &input[i], 1);
			i++;
			continue;
		}
		else if (input[i] == '\"' && !in_single_quotes)
		{
			in_double_quotes = !in_double_quotes;
			if (need_quotes == -1)
				ft_strncat(expanded, &input[i], 1);
			i++;
			continue;
		}
		if (input[i] == '$' && input[i + 1] && !in_single_quotes) // Expand only outside single quotes
		{
			i++;
			if (input[i] == '0')
			{
				ft_strcat(expanded, argv[0]);
				i++;
			}
			else if (input[i] >= '1' && input[i] <= '9')
			{
				ft_strcat(expanded, "");
				i++;
			}
			else if (input[i] == '?')
			{
				exit_status = ft_itoa(last_exit_status);
				ft_strcat(expanded, exit_status);
				free(exit_status);
				i++;
			}
			else if (is_valid_var_char(input[i]))
			{
				j = 0;
				while (is_valid_var_char(input[i]))
					var_name[j++] = input[i++];
				var_name[j] = '\0';
				value = ft_getenv(var_name, ms);
				// fprintf(stderr, "var_name: %s\n", var_name);
				// fprintf(stderr, "value: %s\n", value);
				if (value)
				{
					if ((ft_strchr(value, '"') || ft_strchr(value, '\'')) && need_quotes)
					{
						ft_strcat(expanded, "$");
						ft_strcat(expanded, var_name);
					}
					else if (!need_quotes && !(ft_strchr(value, '"') || ft_strchr(value, '\'')))
					{
						ft_strcat(expanded, "$");
						ft_strcat(expanded, var_name);
					}
					else
						ft_strcat(expanded, value);
				}
				ft_memset(var_name, 0, sizeof(var_name));
				//free(value);
			}
			else
			{
				// If "$" is alone in quotes, preserve it (echo "$")
				if (in_double_quotes && (input[i] == '\'' || input[i] == '\"'))
				{
					ft_strncat(expanded, "$", 1);
					continue;
				}
				// If $ is followed by a quote, do NOT print it
				if (input[i] == '\'' || input[i] == '\"')
					continue;
				// Otherwise, keep $
				ft_strncat(expanded, "$", 1);
				if (input[i]) // Add next character if not at end
				{
					ft_strncat(expanded, &input[i], 1);
					i++;
				}
			}
		}
		else
		{
			ft_strncat(expanded, &input[i], 1);
			i++;
		}
		// fprintf(stderr, "expanded: %s\n", expanded);
	}
	return (expanded);
}
