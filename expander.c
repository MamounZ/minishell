/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaman-alrifai <yaman-alrifai@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 16:44:28 by mazaid            #+#    #+#             */
/*   Updated: 2025/06/05 14:48:22 by mazaid           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	copy_value(t_expand *e)
{
	int	z;

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
int	handle_special_dollar_cases(char *input, t_expand *e, t_ms *ms)
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
		if (!e->exit_status)
			exit_fail_itoa(ms);
		ft_strcat(e->expanded, e->exit_status);
		free(e->exit_status);
		e->i++;
		return (1);
	}
	return (0);
}

int	handle_variable_expansion(char *input, t_expand *e, t_ms *ms)
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

void	handle_unknown_dollar(char *input, t_expand *e)
{
	if (e->in_double_quotes && (input[e->i] == '\'' || input[e->i] == '\"'))
	{
		ft_strncat(e->expanded, "$", 1);
		return ;
	}
	if (input[e->i] == '\'' || input[e->i] == '\"')
		return ;
	ft_strncat(e->expanded, "$", 1);
	if (input[e->i])
		ft_strncat(e->expanded, &input[e->i++], 1);
}

char	*expand_variables(char *input, t_ms *ms)
{
	t_expand	e;

	ft_bzero(&e, sizeof(e));
	e.expanded = malloc(ft_lenexpand(input, ms) + 1);
	if (!e.expanded)
		return (NULL);
	e.expanded[0] = '\0';
	while (input[e.i])
	{
		if (handle_quotes(input, &e))
			continue ;
		if (input[e.i] == '$' && input[e.i + 1] && !e.in_single_quotes)
		{
			e.i++;
			if (handle_special_dollar_cases(input, &e, ms))
				continue ;
			if (handle_variable_expansion(input, &e, ms))
				continue ;
			handle_unknown_dollar(input, &e);
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
// 		if (input[i] == '$' && input[i + 1] && !in_single_quotes)
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
