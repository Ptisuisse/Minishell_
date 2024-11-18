/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lvan-slu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/17 13:05:12 by lvan-slu          #+#    #+#             */
/*   Updated: 2024/10/17 13:05:14 by lvan-slu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*replace_by_exit_code(char *result, int *result_index,
		t_command **command)
{
	char	*exit_code;
	int		i;

	i = 0;
	(void)result;
	(void)result_index;
	exit_code = ft_itoa((*command)->exit_code);
	return (exit_code);
}

void	error_message(const char *token, t_command **cmd)
{
	(*cmd)->exit_code = 2;
	if (token)
		ft_printf_error(" syntax error near unexpected token `%s'\n", token);
}

int	last_exitcode(t_command *command)
{
	t_command	*cmd;
	int			exit_code;

	cmd = command;
	exit_code = 0;
	if (command)
	{
		if (command->next)
		{
			while (command->next)
				command = command->next;
		}
		exit_code = command->exit_code;
	}
	command = cmd;
	return (exit_code);
}
