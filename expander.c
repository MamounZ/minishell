/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mazaid <mazaid@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 16:44:28 by mazaid            #+#    #+#             */
/*   Updated: 2025/06/02 09:46:08 by yaman-alrif      ###   ########.fr       */
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

static void len_copy_value(t_expand *e, int *size)
{
	int z;

	z = 0;
	while (e->value[z])
	{
		if (e->value[z] == '\"' && !e->in_double_quotes)
			*size += 3;
		else if (e->value[z] == '\"' && e->in_double_quotes)
			*size += 5;
		else if (e->value[z] == '\'' && !e->in_double_quotes)
			*size += 3;
		else
			*size += 1;
		z++;
	}
}

static void copy_value(t_expand *e)
{
	int z;

	z = 0;
	while (e->value[z])
	{
		if (e->value[z] == '\"' && !e->in_double_quotes)
			ft_strcat(e->expanded, "\'\"\'");
		else if (e->value[z] == '\"' && e->in_double_quotes)
			ft_strcat(e->expanded, "\"\'\"\'\"");
		else if (e->value[z] == '\'' && !e->in_double_quotes)
			ft_strcat(e->expanded, "\"\'\"");
		else
			ft_strncat(e->expanded, &e->value[z], 1);
		z++;
	}
}

int handle_quotes(char *input, t_expand *e)
{
	if (input[e->i] == '\'' && !e->in_double_quotes)
	{
		e->in_single_quotes = !e->in_single_quotes;
		ft_strncat(e->expanded, &input[e->i++], 1);
		return (1);
	}
	else if (input[e->i] == '\"' && !e->in_single_quotes)
	{
		e->in_double_quotes = !e->in_double_quotes;
		ft_strncat(e->expanded, &input[e->i++], 1);
		return (1);
	}
	return (0);
}

int len_handle_quotes(char *input, t_expand *e)
{
	if (input[e->i] == '\'' && !e->in_double_quotes)
	{
		e->in_single_quotes = !e->in_single_quotes;
		return (1);
	}
	else if (input[e->i] == '\"' && !e->in_single_quotes)
	{
		e->in_double_quotes = !e->in_double_quotes;
		return (1);
	}
	return (0);
}

int handle_special_dollar_cases(char *input, t_expand *e, t_ms *ms)
{
	if (input[e->i] == '0')
	{
		ft_strcat(e->expanded, ms->argv[0]);
		e->i++;
		return (1);
	}
	else if (input[e->i] >= '1' && input[e->i] <= '9')
	{
		e->i++;
		return (1);
	}
	else if (input[e->i] == '?')
	{
		e->exit_status = ft_itoa(ms->last_exit_status);
		ft_strcat(e->expanded, e->exit_status);
		free(e->exit_status);
		e->i++;
		return (1);
	}
	return (0);
}

int len_handle_special_dollar_cases(char *input, t_expand *e, t_ms *ms, int *size)
{
	if (input[e->i] == '0')
	{
		e->i++;
		(*size)++;
		return (1);
	}
	else if (input[e->i] >= '1' && input[e->i] <= '9')
	{
		e->i++;
		return (1);
	}
	else if (input[e->i] == '?')
	{
		*size += ft_strlen(ft_itoa(ms->last_exit_status)); // yaman -> do ft_numlen
		e->i++;
		return (1);
	}
	return (0);
}

int len_handle_variable_expansion(char *input, t_expand *e, t_ms *ms, int *size)
{
	if (is_valid_var_char(input[e->i]))
	{
		e->j = 0;
		while (is_valid_var_char(input[e->i]))
			e->var_name[e->j++] = input[e->i++];
		e->var_name[e->j] = '\0';
		e->value = ft_getenv(e->var_name, ms);
		if (e->value)
			len_copy_value(e, size);
		ft_memset(e->var_name, 0, sizeof(e->var_name));
		return (1);
	}
	return (0);
}
int handle_variable_expansion(char *input, t_expand *e, t_ms *ms)
{
	if (is_valid_var_char(input[e->i]))
	{
		e->j = 0;
		while (is_valid_var_char(input[e->i]))
			e->var_name[e->j++] = input[e->i++];
		e->var_name[e->j] = '\0';
		e->value = ft_getenv(e->var_name, ms);
		if (e->value)
			copy_value(e);
		ft_memset(e->var_name, 0, sizeof(e->var_name));
		return (1);
	}
	return (0);
}

int len_handle_unknown_dollar(char *input, t_expand *e)
{
	int size;
	size = 0;
	if (e->in_double_quotes && (input[e->i] == '\'' || input[e->i] == '\"'))
	{
		return (1);
	}
	if (input[e->i] == '\'' || input[e->i] == '\"')
		return (0);
	size++;
	if (input[e->i])
	{
		size++;
		e->i++;
	}
	return (size);
}

void handle_unknown_dollar(char *input, t_expand *e)
{
	if (e->in_double_quotes && (input[e->i] == '\'' || input[e->i] == '\"'))
	{
		ft_strncat(e->expanded, "$", 1);
		return;
	}
	if (input[e->i] == '\'' || input[e->i] == '\"')
		return;
	ft_strncat(e->expanded, "$", 1);
	if (input[e->i])
		ft_strncat(e->expanded, &input[e->i++], 1);
}
void initiate_exp_variables(t_expand *e)
{
	e->i = 0;
	e->j = 0;
	e->in_single_quotes = 0;
	e->in_double_quotes = 0;
}

int ft_lenexpand(char *input, t_ms *ms)
{
	t_expand e;
	int size;

	size = 0;
	initiate_exp_variables(&e);
	while (input[e.i])
	{
		if (len_handle_quotes(input, &e))
			size++;
		if (input[e.i] == '$' && input[e.i + 1] && !e.in_single_quotes)
		{
			e.i++;
			if (len_handle_special_dollar_cases(input, &e, ms, &size))
				continue;
			if (len_handle_variable_expansion(input, &e, ms, &size))
				continue;
			len_handle_unknown_dollar(input, &e);
			continue;
		}
		else
		{
			size++;
			e.i++;
		}
	}
	return (size);
}

char *expand_variables(char *input, t_ms *ms)
{
	t_expand e;

	initiate_exp_variables(&e);
	e.expanded = malloc(ft_lenexpand(input, ms) + 1);
	if (!e.expanded)
		return (NULL);
	e.expanded[0] = '\0';
	while (input[e.i])
	{
		if(handle_quotes(input, &e))
			continue ;
		if (input[e.i] == '$' && input[e.i + 1] && !e.in_single_quotes)
		{
			e.i++;
			if (handle_special_dollar_cases(input, &e, ms))
				continue;
			if (handle_variable_expansion(input, &e, ms))
				continue;
			handle_unknown_dollar(input, &e);
			continue;
		}
		else
			ft_strncat(e.expanded, &input[e.i++], 1);
	}
	return (e.expanded);
}

// handle echo "\$USER"
// char	*expand_variables(char **argv, char *input, t_ms *ms)
// {
// 	int i;
// 	int j;
// 	char *expanded;
// 	char var_name[256];
// 	char *exit_status;
// 	char *value;
// 	int in_single_quotes = 0;
// 	int in_double_quotes = 0;

// 	expanded = malloc(10000);
// 	if (!expanded)
// 		return NULL;
// 	expanded[0] = '\0';
// 	i = 0;
// 	while (input[i])
// 	{
// 		if (input[i] == '\'' && !in_double_quotes)
// 		{
// 			in_single_quotes = !in_single_quotes;
// 			ft_strncat(expanded, &input[i], 1);
// 			i++;
// 			continue;
// 		}
// 		else if (input[i] == '\"' && !in_single_quotes)
// 		{
// 			in_double_quotes = !in_double_quotes;
// 			ft_strncat(expanded, &input[i], 1);
// 			i++;
// 			continue;
// 		}
// 		if (input[i] == '$' && input[i + 1] && !in_single_quotes) // Expand only outside single quotes
// 		{
// 			i++;
// 			if (input[i] == '0')
// 			{
// 				ft_strcat(expanded, argv[0]);
// 				i++;
// 			}
// 			else if (input[i] >= '1' && input[i] <= '9')
// 			{
// 				ft_strcat(expanded, "");
// 				i++;
// 			}
// 			else if (input[i] == '?')
// 			{
// 				exit_status = ft_itoa(ms->last_exit_status);
// 				ft_strcat(expanded, exit_status);
// 				free(exit_status);
// 				i++;
// 			}
// 			else if (is_valid_var_char(input[i]))
// 			{
// 				j = 0;
// 				while (is_valid_var_char(input[i]))
// 					var_name[j++] = input[i++];
// 				var_name[j] = '\0';
// 				value = ft_getenv(var_name, ms);
// 				if (value)
// 				{
// 						int z = 0;
// 						while (value[z])
// 						{
// 							if (value[z] == '\"' && !in_double_quotes)
// 							{
// 								ft_strcat(expanded, "\'\"\'");
// 							}
// 							else if (value[z] == '"' && in_double_quotes)
// 							{
// 								ft_strcat(expanded, "\"\'\"\'\"");
// 							}
// 							else if (value[z] == '\'' && !in_double_quotes)
// 							{
// 								ft_strcat(expanded, "\"\'\"");
// 							}
// 							else
// 							{
// 								ft_strncat(expanded, &value[z], 1);
// 							}
// 							z++;
// 					}
// 				}
// 				ft_memset(var_name, 0, sizeof(var_name));
// 				//free(value);
// 			}
// 			else
// 			{
// 				// If "$" is alone in quotes, preserve it (echo "$")
// 				if (in_double_quotes && (input[i] == '\'' || input[i] == '\"'))
// 				{
// 					ft_strncat(expanded, "$", 1);
// 					continue;
// 				}
// 				// If $ is followed by a quote, do NOT print it
// 				if (input[i] == '\'' || input[i] == '\"')
// 					continue;
// 				// Otherwise, keep $
// 				ft_strncat(expanded, "$", 1);
// 				if (input[i]) // Add next character if not at end
// 				{
// 					ft_strncat(expanded, &input[i], 1);
// 					i++;
// 				}
// 			}
// 		}
// 		else
// 		{
// 			ft_strncat(expanded, &input[i], 1);
// 			i++;
// 		}
// 		// fprintf(stderr, "expanded: %s\n", expanded);
// 	}
// 	return (expanded);
// }
