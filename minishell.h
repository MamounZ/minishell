/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mazaid <mazaid@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 15:29:07 by mazaid            #+#    #+#             */
/*   Updated: 2025/06/05 18:25:43 by mazaid           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <unistd.h>
# include <stdlib.h>
# include <signal.h>
# include <unistd.h>
# include <readline/history.h>
# include <readline/readline.h>
# include "libft/includes/libft.h"
# include "libft/includes/get_next_line.h"
# include "libft/includes/ft_printf.h"
# include <sys/wait.h>

extern int	g_signal;

typedef enum e_token_type
{
	WORD,
	PIPE,
	REDIR_IN,
	REDIR_OUT,
	HEREDOC,
	APPEND,
	FAIL
}	t_token_type;

typedef struct s_token
{
	char			*value;
	t_token_type	type;
	struct s_token	*next;
}	t_token;

typedef struct s_heredoc
{
	int					fd;
	struct s_heredoc	*n;
}	t_heredoc;

typedef struct s_cmd
{
	char			**args;
	char			*path;
	int				fd_in;
	int				fd_out;
	int				it_is_ok;
	struct s_cmd	*next;
}	t_cmd;

typedef struct s_ms
{
	char		**envp_cpy;
	char		**new_env;
	t_token		*tokens;
	t_cmd		*cmds;
	t_heredoc	*doc;
	char		**argv;
	int			last_exit_status;
	int			err;
	int			line_count;
}	t_ms;

typedef struct s_expand
{
	int		i;
	int		j;
	int		in_single_quotes;
	int		in_double_quotes;
	char	var_name[256];
	char	*expanded;
	char	*exit_status;
	char	*value;
	int		size;
}	t_expand;

void			ft_free_ms(t_ms *ms, int last);
void			close_cmds(t_cmd *cmds);
int				ft_numlen(int n);
void			free_doc(t_heredoc *doc);
void			copy_env(char **envp, t_ms *ms);
void			ft_exit(char **args, t_ms *ms);
char			*expand_variables(char *input, t_ms *ms);
void			execute_builtin(char **args, t_ms *ms);
int				is_builtin(char *cmd);
int				is_valid_var_char(char c);
void			ft_export(char **args, t_ms *ms);
void			ft_env(t_ms *ms);
int				is_valid_identifier(char *str);
void			ft_unset(char **args, t_ms *ms);
void			ft_pwd(void);
char			*ft_getenv(char *var, t_ms *ms);
void			add_to_env(char *arg, t_ms *ms);
void			ft_cd(char **args, t_ms *ms);
char			**realloc_env(char **envp, char *new_var);
void			setup_signals(void);
t_token			*tokenize(char *input, t_ms *ms);
void			print_tokens(t_token *tokens);
t_token			*new_token(char *value, t_token_type type);
int				add_token(t_token **tokens, t_token *new_token, t_ms *ms);
void			free_tokens(t_token *tokens);
int				check_token(t_ms *ms);
void			execute_command(t_ms *ms);
void			free_args(char **args);
void			free_env(char **envp);
void			rm_quote(t_token *ms);
void			exec_cmd(t_ms *ms);
char			*get_cmd_path(char *cmd, t_ms *ms);
void			fill_cmds(t_cmd *ms, t_token *tm, t_ms *m);
void			fill_cmds_file(t_ms *ms);
int				num_of_words(t_token *tmp);
int				is_quote(char c);
char			*tokenize_to_char(t_token *tokens);
void			free_cmds(t_cmd *cmds);
int				handle_special_dollar_cases(char *input, t_expand *e, t_ms *ms);
void			setup_heredoc_signals(void);
void			set_env_value(char *var, char *value, t_ms *ms);
int				len_handle_quotes(char *input, t_expand *e);
int				handle_quotes(char *input, t_expand *e);
int				ft_lenexpand(char *input, t_ms *ms);
void			exit_fail_itoa(t_ms *ms);
int				ft_numlen(int n);
int				check_quotes(char *input);
char			**tokens_to_args(t_token *tokens);
void			print_hredoc(t_heredoc *doc);
int				len_handle_special_dollar_cases_here(char *input,
					t_expand *e, t_ms *ms);
int				len_handle_variable_expansion_heredoc(char *input,
					t_expand *e, t_ms *ms);
void			len_handle_plain_dollar(char *input, t_expand *e);
int				len_heredoc_exp(t_expand *e, char *input, t_ms *ms);
void			add_heredoc(t_ms *ms, int fd);
t_cmd			*create_cmd(t_token *tmp, t_ms *ms);
void			fail_expander(char *input, t_ms *ms);
void			add_cmd(t_ms *ms, t_cmd *cmd);
int				token_size(t_token *tmp);
int				rm_quote_c(char *str);
int				handle_variable_expansion_heredoc(char *input,
					t_expand *e, t_ms *ms);
void			handle_plain_dollar(char *input, t_expand *e);
char			*expand_heredoc(char *input, t_ms *ms);
void			close_and_free_heredoc(t_ms *ms);
void			exit_heredoc(t_ms *ms, char *line, int fd);
int				heredoc_loop(t_token *tmp,
					int pipe_fd[2], int expand, t_ms *ms);
void			dad_heredoc_thing(int pipe_fd, t_ms *ms);
void			fork_heredoc(t_token *tmp,
					int pipe_fd[2], int expand, t_ms *ms);
int				fill_here_doc(t_ms *ms);
void			open_readout(t_token *tmp, t_cmd *cmd);
void			open_readin(t_token *tmp, t_cmd *cmd);
void			open_append(t_token *tmp, t_cmd *cmd);
int				next_cmd(t_token **tm, t_cmd **cmd, t_ms *ms, t_token *tmp);
void			find_in_or_out(t_token *tmp, t_cmd *cmd, t_heredoc **t);
void			cmd_error(t_cmd *cmd, t_ms *ms);
int				there_are_a_douler(char *str);
char			*dup_token(t_token *tmp);
void			fill_cmds_args(char **args, t_token *tm, t_ms *ms);
void			wait_all(pid_t pid, t_ms *ms);
void			in_out_cmds(t_cmd *tmp, int prev_fd, int fd[2]);
void			clean_child(t_cmd *tmp, int prev_fd, t_ms *ms);
void			print_not_found(char *cmd);
void			child_execve(t_cmd *tmp, int prev_fd, t_ms *ms);
void			it_is_not_ok(t_cmd *tmp, int prev_fd, int fd[2], t_ms *ms);
int				ft_isspace(char c);
int				is_operator(char *str);
t_token_type	get_token_type(char *value);
#endif