/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_utiltes.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mazaid <mazaid@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/05 11:46:47 by yaman-alrif       #+#    #+#             */
/*   Updated: 2025/06/05 18:59:14 by mazaid           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_token	*new_token(char *value, t_token_type type)
{
	t_token	*token;

	if (type == FAIL || !value)
		return (NULL);
	token = malloc(sizeof(t_token));
	if (!token)
		return (NULL);
	token->value = value;
	token->type = type;
	token->next = NULL;
	return (token);
}

int	add_token(t_token **tokens, t_token *new_token, t_ms *ms)
{
	t_token	*tmp;

	if (!new_token)
	{
		free_tokens(*tokens);
		ms->err = 1;
		return (0);
	}
	if (!*tokens)
	{
		*tokens = new_token;
		return (1);
	}
	tmp = *tokens;
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = new_token;
	return (1);
}

void	free_tokens(t_token *tokens)
{
	t_token	*tmp;

	while (tokens)
	{
		tmp = tokens;
		tokens = tokens->next;
		free(tmp->value);
		free(tmp);
	}
}

int	check_token(t_ms *ms)
{
	t_token	*tmp;
	t_token	*tmp2;

	tmp = ms->tokens;
	while (tmp)
	{
		tmp2 = tmp->next;
		if (tmp->type == PIPE)
		{
			if (!tmp2 || tmp2->type == PIPE || tmp == ms->tokens)
			{
				ft_printf("minishell: syntax error near unexpected token "
					"`|'\n");
				return (1);
			}
		}
		else if ((tmp->type != WORD) && (!tmp2 || tmp2->type != WORD))
		{
			ft_printf("minishell: syntax error near unexpected token "
				"`newline'\n");
			return (1);
		}
		tmp = tmp->next;
	}
	return (0);
}
