/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mazaid <mazaid@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 15:29:07 by mazaid            #+#    #+#             */
/*   Updated: 2025/03/02 15:23:12 by mazaid           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# ifndef MINISHELL_H
# define MINISHELL_H

#include <unistd.h>
#include <stdlib.h>
#include <readline/history.h>
#include <readline/readline.h>
#include "libft/includes/libft.h"
#include "libft/includes/get_next_line.h"
#include "libft/includes/ft_printf.h"

typedef struct s_ms
{
	char	**envp_cpy;
}		t_ms;

void copy_env(char **envp, t_ms *ms);
char *expand_variables(char **argv, char *input, char **envp, int last_exit_status);
void execute_builtin(char **args, t_ms *ms);

#endif