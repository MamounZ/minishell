/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mazaid <mazaid@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/05 22:06:07 by mazaid            #+#    #+#             */
/*   Updated: 2025/06/05 14:25:38 by mazaid           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	handle_too_many_args(char **args, t_ms *ms)
{
	if (args[1] && args[2])
	{
		printf("minishell: cd: too many arguments\n");
		ms->last_exit_status = 1;
		return (1);
	}
	return (0);
}

void	handle_missing_env_error(const char *var_name, t_ms *ms, char *oldpwd)
{
	printf("minishell: cd: %s not set\n", var_name);
	ms->last_exit_status = 1;
	free(oldpwd);
}

static char	*get_target_dir(char **args, t_ms *ms, char *oldpwd)
{
	char	*dir;

	if (!args[1] || strcmp(args[1], "~") == 0)
	{
		dir = ft_getenv("HOME", ms);
		if (!dir)
		{
			handle_missing_env_error("HOME", ms, oldpwd);
			return (NULL);
		}
	}
	else if (strcmp(args[1], "-") == 0)
	{
		dir = ft_getenv("OLDPWD", ms);
		if (!dir)
		{
			handle_missing_env_error("OLDPWD", ms, oldpwd);
			return (NULL);
		}
		printf("%s\n", dir);
	}
	else
		dir = args[1];
	return (dir);
}

void	ft_cd(char **args, t_ms *ms)
{
	char	*oldpwd;
	char	*newpwd;
	char	*dir;

	if (handle_too_many_args(args, ms))
		return ;
	oldpwd = getcwd(NULL, 0);
	dir = get_target_dir(args, ms, oldpwd);
	if (!dir)
		return ;
	if (chdir(dir) != 0)
	{
		perror("minishell: cd");
		free(oldpwd);
		return ;
	}
	newpwd = getcwd(NULL, 0);
	set_env_value("OLDPWD", oldpwd, ms);
	set_env_value("PWD", newpwd, ms);
	free(oldpwd);
	free(newpwd);
}

// void ft_cd(char **args, t_ms *ms)
// {
// 	char *dir;
// 	char *oldpwd;
// 	char *newpwd;

// 	if (args[1] && args[2])
// 	{
// 		printf("minishell: cd: too many arguments\n");
// 		ms->last_exit_status = 1;
// 		return;
// 	}
// 	oldpwd = getcwd(NULL, 0);
// 	if (!args[1] || strcmp(args[1], "~") == 0) // Default to HOME
// 	{
// 		dir = ft_getenv("HOME", ms);
// 		if (!dir)
// 		{
// 			printf("minishell: cd: HOME not set\n");
// 			ms->last_exit_status = 1;
// 			free(oldpwd);
// 			return;
// 		}
// 	}
// 	else if (strcmp(args[1], "-") == 0) // Go to OLDPWD
// 	{
// 		dir = ft_getenv("OLDPWD", ms);
// 		if (dir)
// 			printf("%s\n", dir); // Print the new path when using `cd -`
// 		else
// 		{
// 			ms->last_exit_status = 1;
// 			printf("minishell: cd: OLDPWD not set\n");
// 			free(oldpwd);
// 			return;
// 		}
// 	}
// 	else
// 		dir = args[1];

// 	if (chdir(dir) != 0)
// 	{
// 		perror("minishell: cd");
// 		free(oldpwd);
// 		return;
// 	}
// 	newpwd = getcwd(NULL, 0);
// 	set_env_value("OLDPWD", oldpwd, ms);
// 	set_env_value("PWD", newpwd, ms);
// 	free(oldpwd);
// }
