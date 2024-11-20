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

int		g_received_signal = 0;

void	close_fd(void)
{
	int	i;

	i = 3;
	while (i < 1024)
	{
		close(i);
		i++;
	}
}

void	ft_prompt(int save_exit_code, t_command *command_list, t_env **env_list,
		char *input)
{

	//command_list = init_command(save_exit_code, env_list);
	while (1)
	{
		setup_signal_handling();
		handle_received_signal(&save_exit_code);
		input = handle_input(input, save_exit_code, env_list, &command_list);
		if (process_input(&command_list, input, &save_exit_code, env_list))
		{
			if (command_list->error_file > 0 && command_list->next == NULL)
				check_error_file(command_list);
			else if (just_a_path(command_list))
			{
				check_heredoc(command_list);
				select_type(command_list, env_list);
			}
		}
		save_exit_code = last_exitcode(command_list);
		cleanup(&command_list, input);
		close_fd();
	}
}

int	main(int argc, char **argv, char **envp)
{
	t_command	*command_list;
	t_env		*env_list;
	char		*input;
	int			save_exit_code;

	input = NULL;
	save_exit_code = 0;
	(void)argc;
	(void)argv;
	create_env_list(envp, &env_list);
	command_list = NULL;
	ft_prompt(save_exit_code, command_list, &env_list, input);
	free_env_list(&env_list);
	return (0);
}
