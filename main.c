/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaman-alrifai <yaman-alrifai@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 15:28:51 by mazaid            #+#    #+#             */
/*   Updated: 2025/03/20 11:48:03 by yaman-alrif      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"


// checj any quotes in the input
int check_quotes(char *input)
{
	int num = 0;
	while (*input)
	{
		if (*input == '"' || *input == '\'')
		{
			int quote = *input;
			num++;
			input++;
			while (*input && *input != quote)
				input++;
			if (*input == quote)
				num++;
		}
		if (*input)
			input++;
	}
	if (num % 2)
	{
		ft_printf("minishell: syntax error: unexpected end of file\n");
		return (1);
	}
	return (0);
}
// Convert token list into an array of arguments
char **tokens_to_args(t_token *tokens)
{
	int count;
	int i;
	t_token *tmp;

	count = 0;
	i = 0;
	tmp = tokens;
	while (tmp)
	{
		count++;
		tmp = tmp->next;
	}
	char **args = malloc(sizeof(char *) * (count + 1));
	if (!args)
		return (NULL);
	tmp = tokens;
	while (i < count)
	{
		args[i] = ft_strdup(tmp->value);
		tmp = tmp->next;
		i++;
	}
	args[count] = NULL;
	return (args);
}

void free_args(char **args)
{
	int i = 0;
	while (args[i])
		free(args[i++]);
	free(args);
}

int main(int argc, char **argv, char **envp)
{
	char	*input;
	char *expanded_input;
	char **cmd_args;
	t_ms	*ms;

	(void) argv;
	(void) argc;
	ms = malloc(sizeof(t_ms));
	if (!ms)
		return (0);
	ms->envp_cpy = NULL;
	copy_env(envp, ms);
	setup_signals();
	while (1)
	{
		input = readline("minishell> ");
		if (!input || ft_strcmp(input, "exit") == 0)
		{
			printf("exit\n");
			break;
		}
		if (*input)
			add_history(input);
		if (check_quotes(input))
		{
			free(input);
			continue;
		}
		copy_env(envp, ms);
		expanded_input = expand_variables(argv, input, ms, 1);
		// ft_printf("%s\n", expanded_input);
		ms->tokens = tokenize(expanded_input);
		check_token(ms);
		// print_tokens(ms->tokens);
		cmd_args = tokens_to_args(ms->tokens);
		// Execute builtin command if it matches
		if (cmd_args && cmd_args[0] && is_builtin(cmd_args[0]))
			execute_builtin(cmd_args, ms);
		// If not built-in, execute as an external command (later)
		else
			printf("Not a built-in command: %s\n", cmd_args[0]);
		free(input);
		free(expanded_input);
		free_args(cmd_args);
	}
	return (0);
}
