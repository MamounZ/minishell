/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mazaid <mazaid@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 15:29:07 by mazaid            #+#    #+#             */
/*   Updated: 2025/02/26 19:03:24 by mazaid           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# ifndef MINISHELL_H
# define MINISHELL_H

#include <unistd.h>
#include <stdlib.h>
#include <readline/history.h>
#include <readline/readline.h>

typedef struct s_ms
{
	char	**envp_cpy;
}		t_ms;

void copy_env(char **envp, t_ms *ms);

#endif