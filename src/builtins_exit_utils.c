/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_exit_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lvan-slu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/14 15:43:29 by lvan-slu          #+#    #+#             */
/*   Updated: 2024/11/14 15:43:31 by lvan-slu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_isspace(char c)
{
	return (c == ' ' || (c >= '\t' && c <= '\r'));
}

int	is_numeric(const char *str)
{
	int	i;

	i = 0;
	if (str[0] == '-' || str[0] == '+')
		i++;
	while (str[i])
	{
		if (!ft_isdigit(str[i]))
			return (0);
		i++;
	}
	return (1);
}

void	handle_exit_error(char *arg)
{
	ft_printf_error("exit: %s: numeric argument required\n", arg);
	ft_printf("exit\n");
	exit(2);
}
