/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_echo.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lvan-slu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/17 13:04:15 by lvan-slu          #+#    #+#             */
/*   Updated: 2024/10/17 13:04:18 by lvan-slu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	echo_cmd(t_command *command)
{
	int	newline;
	int	i;
	int	j;

	newline = 1;
	i = 1;
	j = 0;
	while (command->args[i] && command->args[i][0] == '-')
	{
		j = 1;
		while (command->args[i][j] == 'n')
			j++;
		if (command->args[i][j] == '\0')
			newline = 0;
		else
			break ;
		i++;
	}
	echo_print(command->args, i);
	if (newline)
		ft_printf("\n");
	command->exit_code = 0;
}

int	echo_print(char **args, int i)
{
	while (args[i])
	{
		ft_printf("%s", args[i++]);
		if (args[i])
			ft_printf(" ");
	}
	return (i);
}
