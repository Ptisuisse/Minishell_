/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_clear_exit.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lvan-slu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/17 13:04:05 by lvan-slu          #+#    #+#             */
/*   Updated: 2024/10/17 13:04:08 by lvan-slu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	clear_cmd(void)
{
	system("clear");
}

int	exit_cmd(t_command *command)
{
	if (ft_strcmp(command->args[0], "exit") == 0 && command->args[2])
	{
		ft_printf(" too many arguments\n");
		g_exit_code = 1;
	}
	else if (ft_strcmp(command->args[0], "exit") == 0 && command->args[1])
	{
		if (ft_isdigit(command->args[1][0]))
			g_exit_code = ft_atoi(command->args[1]);
		else if (command->args[1][0] == '-')
			g_exit_code = 156;
		else if (ft_isalpha(command->args[1][0]))
		{
			ft_printf(" numeric argument required\n");
			g_exit_code = 2;
		}
		else
			g_exit_code = 255;
	}
	exit(g_exit_code);
}
