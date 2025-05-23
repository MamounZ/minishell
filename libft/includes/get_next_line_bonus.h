/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_bonus.h                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dkhalil <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/13 00:54:43 by mazaid            #+#    #+#             */
/*   Updated: 2024/10/03 07:21:52 by mazaid           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#ifndef GET_NEXT_LINE_BONUS_H
# define GET_NEXT_LINE_BONUS_H

# include <unistd.h>
# include <stdlib.h>
# include <fcntl.h>
# include <stdint.h>

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 8
# endif

size_t	ft_strlen(const char *str);
char	*newline_exist(char *str);
char	*ft_strjoin(char *left, char *buff);
void	*ft_calloc(size_t count, size_t size);
char	*get_next_line(int fd);

#endif
