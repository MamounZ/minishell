/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mazaid <mazaid@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/21 19:46:02 by yaman-alrif       #+#    #+#             */
/*   Updated: 2025/06/05 17:59:03 by mazaid           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	word_token(char **input, t_token_type *type)
{
	char	quote_char;

	if (is_quote(**input))
	{
		quote_char = *(*input)++;
		while (**input && **input != quote_char)
			(*input)++;
		if (**input == quote_char)
			(*input)++;
	}
	while (**input && !is_operator(*input) && !ft_isspace(**input))
	{
		if (is_quote(**input))
		{
			quote_char = *(*input)++;
			while (**input && **input != quote_char)
				(*input)++;
			if (**input == quote_char)
				(*input)++;
			continue ;
		}
		(*input)++;
	}
	*type = WORD;
}

void	skip_spaces(char **input)
{
	while (**input && ft_isspace(**input))
		(*input)++;
}

t_token	*tokenize(char *input, t_ms *ms)
{
	t_token			*tokens;
	char			*token;
	int				operator_len;
	t_token_type	type;

	tokens = NULL;
	while (*input)
	{
		skip_spaces(&input);
		if (!*input)
			break ;
		token = input;
		operator_len = is_operator(input);
		if (operator_len)
		{
			input += operator_len;
			type = get_token_type(ft_substr(token, 0, operator_len));
		}
		else
			word_token(&input, &type);
		if (!add_token(&tokens, new_token
				(ft_substr(token, 0, input - token), type), ms))
			return (NULL);
	}
	return (tokens);
}

void	print_tokens(t_token *tokens)
{
	t_token	*tmp;

	tmp = tokens;
	while (tmp)
	{
		printf("Token: %-10s Type: %d\n", tmp->value, tmp->type);
		tmp = tmp->next;
	}
}

void	rm_quote(t_token *ms)
{
	t_token	*tmp;
	int		in_quotes;
	int		i;
	int		j;

	tmp = ms;
	in_quotes = 0;
	while (tmp)
	{
		i = -1;
		j = 0;
		if (tmp->type == WORD)
		{
			while (tmp->value[++i])
				if (is_quote(tmp->value[i]) && !in_quotes)
					in_quotes = tmp->value[i];
			else if (in_quotes && tmp->value[i] == in_quotes)
				in_quotes = 0;
			else
				tmp->value[j++] = tmp->value[i];
		}
		tmp->value[j] = '\0';
		tmp = tmp->next;
	}
}
