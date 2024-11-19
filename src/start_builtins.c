/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   start_builtins.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lisambet <lisambet@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/19 13:23:11 by lisambet          #+#    #+#             */
/*   Updated: 2024/11/19 13:24:46 by lisambet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	is_builtin_command(const char *command)
{
	if (!command || ft_strcmp(command, "") == 0)
		return (0);
	if ((ft_strcmp(command, "exit") == 0 && ft_strlen(command) == 4)
		|| (ft_strcmp(command, "echo") == 0 && ft_strlen(command) == 4)
		|| (ft_strcmp(command, "cd") == 0 && ft_strlen(command) == 2)
		|| (ft_strcmp(command, "export") == 0 && ft_strlen(command) == 6)
		|| (ft_strcmp(command, "unset") == 0 && ft_strlen(command) == 5)
		|| (ft_strcmp(command, "env") == 0 && ft_strlen(command) == 3)
		|| (ft_strcmp(command, "pwd") == 0 && ft_strlen(command) == 3))
		return (1);
	return (0);
}

int	check_builtins(t_command *command, t_env **env_list)
{
	int	status;

	status = is_builtin_command(command->args[0]);
	if (status == 1)
		start_builtins(command, env_list);
	return (status);
}