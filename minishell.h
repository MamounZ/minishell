/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaman-alrifai <yaman-alrifai@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 15:29:07 by mazaid            #+#    #+#             */
/*   Updated: 2025/03/07 12:29:18 by yaman-alrif      ###   ########.fr       */
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
char *expand_variables(char **argv, char *input, char **envp, int last_exit_status);
void execute_builtin(char **args, t_ms *ms);
t_token *tokenize(char *input);
void print_tokens(t_token *tokens);
t_token *new_token(char *value, t_token_type type);
void add_token(t_token **tokens, t_token *new_token);
void free_tokens(t_token *tokens);

#endif