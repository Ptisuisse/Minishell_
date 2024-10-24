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

//int	g_exit_code = 0;

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
	int save_exit_code = 0;
	t_command	*command_list;
	t_env		*env_list;

	(void)argc;
	(void)argv;
	env_list = malloc(sizeof(t_env));
	//command_list = malloc(sizeof(t_command));
	command_list = NULL;
	create_env_list(envp, &env_list);
	while (1)
	{
		setup_signal_handling();
		input = readline("Minishell > ");
		if (g_received_signal == SIGINT)
			save_exit_code = 130;
		if (input && *input)
			add_history(input);
		if (parse_command_line(input, &command_list, save_exit_code))
		{
			command_list->exit_code = 1;
		}
		else
		{
		//	check_heredoc(command_list);
			if (ft_isprint(*input))
				commands_manager(command_list, &env_list);
		}
		save_exit_code = last_exitcode(command_list);
		free(input);
		free_command_list(command_list);
		command_list = NULL;
		input = NULL;
	}
	return (0);
}
