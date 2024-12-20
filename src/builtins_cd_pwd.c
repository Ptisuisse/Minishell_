/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_cd_pwd.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lvan-slu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/17 13:03:48 by lvan-slu          #+#    #+#             */
/*   Updated: 2024/10/17 13:03:52 by lvan-slu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	update_env(t_env *env_list, char *name, char *value)
{
	t_env	*head;

	head = env_list;
	while (env_list)
	{
		if (!(ft_strcmp(env_list->name, name)))
		{
			free(env_list->value);
			env_list->value = ft_strdup(value);
			break ;
		}
		env_list = env_list->next;
	}
	env_list = head;
}

void	cd_cmd(t_command *command, t_env *env_list)
{
	char	*oldpwd;
	char	*newpwd;

	oldpwd = getcwd(NULL, 0);
	if (!command->args[1])
		command->args[1] = ft_strdup(getenv("HOME"));
	if (command->args[2])
	{
		ft_printf_error("Minishell: cd: too many arguments\n");
		command->exit_code = 1;
	}
	else if (chdir(command->args[1]) < 0)
	{
		ft_printf_error("Minishell: cd: %s: No such file or directory\n",
			command->args[1]);
		command->exit_code = 1;
	}
	else
		command->exit_code = 0;
	newpwd = getcwd(NULL, 0);
	update_env(env_list, "PWD", newpwd);
	update_env(env_list, "OLDPWD", oldpwd);
	free(oldpwd);
	free(newpwd);
}

void	pwd_cmd(t_command *command)
{
	char	*cwd;

	cwd = getcwd(NULL, 0);
	if (cwd)
	{
		ft_printf("%s\n", cwd);
		free(cwd);
		command->exit_code = 0;
	}
	else
	{
		ft_printf_error("Error : pwd\n");
		command->exit_code = 1;
	}
}
