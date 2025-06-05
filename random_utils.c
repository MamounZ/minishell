/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   random_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mazaid <mazaid@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/05 16:12:32 by mazaid            #+#    #+#             */
/*   Updated: 2025/06/05 17:17:37 by mazaid           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	check_quotes(char *input)
{
	int	num;
	int	quote;

	num = 0;
	while (*input)
	{
		if (*input == '"' || *input == '\'')
		{
			quote = *input;
			num++;
			input++;
			while (*input && *input != quote)
				input++;
			if (*input == quote)
				num++;
		}
		if (*input)
			input++;
	}
	if (num % 2)
	{
		ft_printf("minishell: syntax error: unexpected end of file\n");
		return (1);
	}
	return (0);
}

char	**tokens_to_args(t_token *tokens)
{
	int		count;
	int		i;
	t_token	*tmp;
	char	**args;

	count = 0;
	i = 0;
	tmp = tokens;
	while (tmp)
	{
		count++;
		tmp = tmp->next;
	}
	args = malloc(sizeof(char *) * (count + 1));
	if (!args)
		return (NULL);
	tmp = tokens;
	while (i < count)
	{
		args[i] = ft_strdup(tmp->value);
		tmp = tmp->next;
		i++;
	}
	args[count] = NULL;
	return (args);
}

void	free_args(char **args)
{
	int	i;

	i = 0;
	if (!args)
		return ;
	while (args[i])
		free(args[i++]);
	free(args);
}

void	print_hredoc(t_heredoc *doc)
{
	t_heredoc	*tmp;

	tmp = doc;
	while (tmp)
	{
		printf("fd: %d\n", tmp->fd);
		tmp = tmp->n;
	}
}

void	ft_free_ms(t_ms *ms, int last)
{
	free_tokens(ms->tokens);
	ms->tokens = NULL;
	free_cmds(ms->cmds);
	ms->cmds = NULL;
	free_doc(ms->doc);
	ms->doc = NULL;
	ms->err = 0;
	if (last)
	{
		free_args(ms->envp_cpy);
		ms->envp_cpy = NULL;
		free_args(ms->new_env);
		ms->new_env = NULL;
		free(ms);
	}
}
