/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaman-alrifai <yaman-alrifai@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/21 19:46:02 by yaman-alrif       #+#    #+#             */
/*   Updated: 2025/05/30 10:37:05 by yaman-alrif      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int ft_isspace(char c)
{
    return (c == ' ' || c == '\t' || c == '\n' || c == '\v' || c == '\f' || c == '\r');
}

int is_operator(char *str) {
    if (!strncmp(str, "<<", 2) || !strncmp(str, ">>", 2))
        return 2;
    if (strchr("|<>", str[0]))
        return 1;
    return 0;
}

int is_quote(char c) {
    return (c == '"' || c == '\'');
}

t_token_type get_token_type(char *value)
{
    t_token_type type;

    if (!value)
        return (FAIL);
    type = WORD;
    if (!strcmp(value, "|")) type = PIPE;
    if (!strcmp(value, "<")) type = REDIR_IN;
    if (!strcmp(value, ">")) type = REDIR_OUT;
    if (!strcmp(value, ">>")) type = APPEND;
    if (!strcmp(value, "<<")) type = HEREDOC;
    free(value);
    return type;
}

void word_token(char **input, t_token_type *type)
{
    char quote_char;

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
            continue;
        }
        (*input)++;
    }
    *type = WORD;
}

t_token *tokenize(char *input, t_ms *ms)
{
    t_token *tokens;
    char *token;
    int operator_len;
    t_token_type type;

    tokens = NULL;
    while (*input)
    {
        while (*input && ft_isspace(*input))
            input++;
        if (!*input)
            break;
        token = input;
        operator_len = is_operator(input);
        if (operator_len)
        {
            input += operator_len;
            type = get_token_type(ft_substr(token, 0, operator_len));
        }
        else
            word_token(&input, &type);
        if (!add_token(&tokens, new_token(ft_substr(token, 0, input - token), type), ms))
            return (NULL);
    }
    return tokens;
}

void print_tokens(t_token *tokens) {
    t_token *tmp = tokens;
    while (tmp) {
        printf("Token: %-10s Type: %d\n", tmp->value, tmp->type);
        tmp = tmp->next;
    }
}

void rm_quote(t_token *ms)
{
    t_token *tmp;
    int in_quotes;
    int i;
    int j;

    tmp = ms;
    in_quotes = 0;
    while (tmp)
    {
        i = -1;
        j = 0;
        if (tmp->type == WORD)
            while (tmp->value[++i])
            {
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
