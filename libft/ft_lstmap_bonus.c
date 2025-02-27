/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstmap.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mazaid <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/31 20:51:27 by mazaid            #+#    #+#             */
/*   Updated: 2024/09/01 00:07:58 by mazaid           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_list	*ft_lstmap(t_list *lst, void *(*f)(void *), void (*del)(void *))
{
	t_list	*new_lst;
	t_list	*node;
	void	*x;

	if (!lst || !f || !del)
		return (NULL);
	new_lst = NULL;
	while (lst)
	{
		x = (*f)(lst->content);
		node = ft_lstnew(x);
		if (!node)
		{
			del(x);
			ft_lstclear(&new_lst, (*del));
			return (NULL);
		}
		ft_lstadd_back(&new_lst, node);
		lst = lst->next;
	}
	return (new_lst);
}
