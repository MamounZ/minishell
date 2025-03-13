/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mazaid <mazaid@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 15:28:51 by mazaid            #+#    #+#             */
/*   Updated: 2025/03/12 15:30:17 by mazaid           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
		expanded_input = expand_variables(argv, input, ms, 1);
		cmd_args = ft_split(expanded_input, ' '); // Here you should replaice this line with your Tokenizer so the builtin commands could be executede.

		// Execute builtin command if it matches
		if (cmd_args[0])
			execute_builtin(cmd_args, ms);
		free(input);
		free(expanded_input);
	}
	return (0);
}
