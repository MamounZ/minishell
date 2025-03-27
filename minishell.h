/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaman-alrifai <yaman-alrifai@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 15:29:07 by mazaid            #+#    #+#             */
/*   Updated: 2025/03/27 11:08:12 by yaman-alrif      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# ifndef MINISHELL_H
# define MINISHELL_H

#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <readline/history.h>
#include <readline/readline.h>
#include "libft/includes/libft.h"
#include "libft/includes/get_next_line.h"
#include "libft/includes/ft_printf.h"
#include <sys/wait.h>


typedef enum e_token_type {
    WORD, PIPE, REDIR_IN, REDIR_OUT, HEREDOC, APPEND
} t_token_type;

typedef struct s_token {
    char *value;
    t_token_type type;
    struct s_token *next;
} t_token;


typedef struct s_ms
{
	char	**envp_cpy;
	t_token *tokens;
}		t_ms;

void copy_env(char **envp, t_ms *ms);
char *expand_variables(char **argv, char *input, t_ms *ms, int last_exit_status);
void execute_builtin(char **args, t_ms *ms);
int is_builtin(char *cmd);
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
void setup_signals(void);
t_token *tokenize(char *input);
void print_tokens(t_token *tokens);
t_token *new_token(char *value, t_token_type type);
void add_token(t_token **tokens, t_token *new_token);
void free_tokens(t_token *tokens);
int check_token(t_ms *ms);
void execute_command(t_ms *ms);
void free_args(char **args);
#endif