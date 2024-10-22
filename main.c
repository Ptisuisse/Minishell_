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

int	g_exit_code = 0;

int	main(int argc, char **argv, char **envp)
{
	char		*input;
	t_command	*command_list;
	t_env		*env_list;

	(void)argc;
	(void)argv;
	env_list = malloc(sizeof(t_env));
	command_list = NULL;
	command_list = malloc(sizeof(t_command));
	create_env_list(envp, &env_list);
	while (1)
	{
		signal_handle();
		input = readline("Minishell > ");
		if (input && *input)
			add_history(input);
		if (parse_command_line(input, &command_list))
		{
			//error_message(input, command_list);
			free(input);
			return (2);
		}
		check_heredoc(command_list);
		if (ft_isprint(*input))
			commands_manager(command_list, &env_list);
		//print_commands(command_list);
		free(input);
		//free_command_list(command_list);
		command_list = NULL;
	}
	return (0);
}
