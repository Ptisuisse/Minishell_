/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lvan-slu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/17 13:05:26 by lvan-slu          #+#    #+#             */
/*   Updated: 2024/10/17 13:05:27 by lvan-slu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	last_exitcode(t_command *command)
{
	t_command	*cmd;
	int	exit_code;

	cmd = command;
	exit_code = 0;
	while (command->next)
		command = command->next;
	exit_code = command->exit_code;
	command = cmd;
	return (exit_code);
}

int	main(int argc, char **argv, char **envp)
{
	char		*input;
	int			save_exit_code;
	t_command	*command_list;
	t_env		*env_list;

	save_exit_code = 0;
	(void)argc;
	(void)argv;
	env_list = malloc(sizeof(t_env));
	command_list = NULL;
	create_env_list(envp, &env_list);
	while (1)
	{
		setup_signal_handling();
		input = readline("Minishell > ");
		if (handle_received_signal(&save_exit_code))
			printf("globale %d\n", save_exit_code);
		if (g_received_signal == SIGSEGV)
		{
		ft_printf("exit\n");
		exit(1);
		}
		if (input && *input)
			add_history(input);
		process_input(&command_list, &env_list, input, &save_exit_code);
		free(input);
		input = NULL;
	}
	return (0);
}