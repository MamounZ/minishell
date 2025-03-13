/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaman-alrifai <yaman-alrifai@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 15:28:51 by mazaid            #+#    #+#             */
<<<<<<< HEAD
/*   Updated: 2025/03/08 14:22:04 by mazaid           ###   ########.fr       */
=======
/*   Updated: 2025/03/07 11:43:55 by yaman-alrif      ###   ########.fr       */
>>>>>>> Yaman_work
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int main(int argc, char **argv, char **envp)
{
	char	*input;
	char *expanded_input;
	// char **cmd_args;
	t_ms	*ms;

	(void) argv;
	(void) argc;
	ms = malloc(sizeof(t_ms));
	if (!ms)
		return (0);
	ms->envp_cpy = NULL;
	copy_env(envp, ms);
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
<<<<<<< HEAD
		expanded_input = expand_variables(argv, input, ms, 1);
		cmd_args = ft_split(expanded_input, ' '); // Here you should replaice this line with your Tokenizer so the builtin commands could be executede.

		// Execute builtin command if it matches
		if (cmd_args[0])
			execute_builtin(cmd_args, ms);
=======
		copy_env(envp, ms);
		expanded_input = expand_variables(argv, input, ms->envp_cpy, 1);
		ms->tokens = tokenize(expanded_input); // Here you should replaice this line with your Tokenizer so the builtin commands could be executede.
		print_tokens(ms->tokens);

		// Execute builtin command if it matches
		// if (cmd_args[0])
		// 	execute_builtin(cmd_args, ms);

>>>>>>> Yaman_work
		free(input);
		free(expanded_input);
	}
	return (0);
}
