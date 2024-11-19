/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lisambet <lisambet@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/17 13:05:26 by lvan-slu          #+#    #+#             */
/*   Updated: 2024/11/19 17:24:13 by lisambet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	g_received_signal = 0;

int	main(int argc, char **argv, char **envp)
{
	t_command	*command_list;
	t_env		*env_list;
	char		*input;
	int			save_exit_code;

	command_list = NULL;
	env_list = NULL;
	input = NULL;
	save_exit_code = 0;
	(void)argc;
	(void)argv;
	setup_environment(&env_list, envp);
	while (1)
	{
		setup_signal_handling();
		handle_received_signal(&save_exit_code);
		handle_input(&input, save_exit_code, &env_list, &command_list);
		process_commands(&command_list, input, &save_exit_code, &env_list);
		cleanup(&command_list, &input);
	}
	free_env_list(&env_list);
	return (0);
}
