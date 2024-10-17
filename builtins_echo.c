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

void	echo_cmd(char **args)
{
	int	newline;
	int	i;
	int	j;

	newline = 1;
	i = 1;
	j = 0;
	while (args[i] && args[i][0] == '-')
	{
		j = 1;
		while (args[i][j] == 'n')
			j++;
		if (args[i][j] == '\0')
			newline = 0;
		else
			break ;
		i++;
	}
	echo_print(args, i);
	if (newline)
		printf("\n");
	g_exit_code = 0;
}

int	echo_print(char **args, int i)
{
	while (args[i])
	{
		printf("%s", args[i++]);
		if (args[i])
			printf(" ");
	}
	return (i);
}
