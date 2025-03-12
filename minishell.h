/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mazaid <mazaid@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 15:29:07 by mazaid            #+#    #+#             */
/*   Updated: 2025/03/05 22:51:15 by mazaid           ###   ########.fr       */
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
char *expand_variables(char **argv, char *input, t_ms *ms, int last_exit_status);
void execute_builtin(char **args, t_ms *ms);
int	is_valid_var_char(char c);
void ft_export(char **args, t_ms *ms);
void ft_env(t_ms *ms);
int is_valid_identifier(char *str);
void ft_unset(char **args, t_ms *ms);
void ft_pwd(void);
char *ft_getenv(char *var, t_ms *ms);
void add_to_env(char *arg, t_ms *ms);
void ft_cd(char **args, t_ms *ms);
char **realloc_env(char **envp, char *new_var);
#endif