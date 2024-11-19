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

void	handle_exit_code(char *arg, int *exit_code)
{
	long	exit_code_long;

	exit_code_long = atol(arg);
	if (exit_code_long > 9223372036854775807
		|| exit_code_long < (-9223372036854775807))
		handle_exit_error(arg);
	*exit_code = (int)exit_code_long;
}

int	check_and_handle_exit_args(t_command *command)
{
	char	*arg;

	arg = command->args[1];
	while (*arg && ft_isspace(*arg))
		arg++;
	if (*arg == '\0' || !is_numeric(arg))
		handle_exit_error(command->args[1]);
	handle_exit_code(arg, &command->exit_code);
	if (command->args[2])
	{
		ft_printf_error("exit: too many arguments\n");
		command->exit_code = 1;
		return (1);
	}
	return (0);
}

int	exit_cmd(t_command *command, t_env **env_list, int save_exit_code)
{
	if (command && command->args[0] && ft_strcmp(command->args[0], "exit") == 0)
	{
		if (command->args[1])
		{
			if (check_and_handle_exit_args(command))
				return (1);
		}
		else
			command->exit_code = save_exit_code;
		ft_printf("exit\n");
		exit(command->exit_code);
	}
	ft_printf("exit\n");
	free_env_list(env_list);
	free_command_list(&command);
	exit(save_exit_code);
	return (0);
}
