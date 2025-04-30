/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exu_2.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaman-alrifai <yaman-alrifai@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 19:05:25 by yaman-alrif       #+#    #+#             */
/*   Updated: 2025/04/29 19:06:10 by yaman-alrif      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char *tokenize_to_char(t_token *tokens)
{
    char *str;
    int len = 0;
    t_token *tmp = tokens;

    while (tmp)
    {
        len += ft_strlen(tmp->value) + 1; // +1 for space
        tmp = tmp->next;
    }
    str = malloc(sizeof(char) * (len + 1));
    if (!str)
        return (NULL);
    str[0] = '\0';
    tmp = tokens;
    while (tmp)
    {
        strcat(str, tmp->value);
        strcat(str, " ");
        tmp = tmp->next;
    }
    str[len - 1] = '\0'; // remove last space
    return (str);
}
