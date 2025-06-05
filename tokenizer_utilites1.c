/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_utilites1.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mazaid <mazaid@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/05 17:34:41 by mazaid            #+#    #+#             */
/*   Updated: 2025/06/05 17:37:28 by mazaid           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_isspace(char c)
{
	return (c == ' ' || c == '\t' || c == '\n' || c == '\v'
		|| c == '\f' || c == '\r');
}

int	is_operator(char *str)
{
	if (!strncmp(str, "<<", 2) || !strncmp(str, ">>", 2))
		return (2);
	if (strchr("|<>", str[0]))
		return (1);
	return (0);
}

int	is_quote(char c)
{
	return (c == '"' || c == '\'');
}

t_token_type	get_token_type(char *value)
{
	t_token_type	type;

	if (!value)
		return (FAIL);
	type = WORD;
	if (!strcmp(value, "|"))
		type = PIPE;
	if (!strcmp(value, "<"))
		type = REDIR_IN;
	if (!strcmp(value, ">"))
		type = REDIR_OUT;
	if (!strcmp(value, ">>"))
		type = APPEND;
	if (!strcmp(value, "<<"))
		type = HEREDOC;
	free(value);
	return (type);
}
