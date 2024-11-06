/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_exit.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lvan-slu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/17 13:04:05 by lvan-slu          #+#    #+#             */
/*   Updated: 2024/11/06 11:51:44 by lvan-slu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	exit_cmd(t_command *command, t_env **env_list)
{
	(void)env_list;
	if (command)
	{
		if (command->args[0])
		{
			if (ft_strcmp(command->args[0], "exit") == 0 && command->args[2])
			{
				ft_printf_error(" too many arguments\n");
				command->exit_code = 1;
			}
			else if (ft_strcmp(command->args[0], "exit") == 0
				&& command->args[1])
			{
				if (ft_isdigit(command->args[1][0]))
					command->exit_code = ft_atoi(command->args[1]);
				else if (command->args[1][0] == '-')
					command->exit_code = 156;
				else if (ft_isalpha(command->args[1][0]))
				{
					ft_printf_error(" numeric argument required\n");
					command->exit_code = 2;
				}
				else
					command->exit_code = 255;
			}
		}
		exit(command->exit_code);
		free_command_list(command);
	}
	else
	{
		ft_printf("exit\n");
		exit(1);
	}
}
