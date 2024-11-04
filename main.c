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

int g_received_signal = 0;

int	is_executable(t_command *command)
{
	if (command->args[0] || command->file > 0)
		return 1;
	return 0;
}

void	free_envlist(t_env *env_list)
{
	t_env	*tmp;

	while (env_list)
	{
		tmp = env_list;
		env_list = env_list->next;
		free(tmp->name);
		free(tmp->value);
		free(tmp);
	}
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
		handle_received_signal(&save_exit_code);
		if (input && *input)
			add_history(input);
		process_input(&command_list, &env_list, input, &save_exit_code);
		if (is_executable(command_list) && save_exit_code != 256)
		{
			check_heredoc(command_list);
			select_type(command_list, &env_list);
		}
		save_exit_code = last_exitcode(command_list);
		free_command_list(command_list);
		command_list = NULL;
		free(input);
		input = NULL;
	}
	//free_envlist(env_list);
	return (0);
}
