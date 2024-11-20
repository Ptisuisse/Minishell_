/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setting.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lisambet <lisambet@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/19 17:04:27 by lisambet          #+#    #+#             */
/*   Updated: 2024/11/20 13:22:40 by lisambet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	setup_environment(t_env **env_list, char **envp)
{
	*env_list = malloc(sizeof(t_env));
	if (!*env_list)
		exit(EXIT_FAILURE);
	create_env_list(envp, env_list);
}

char	*handle_input(char *input, int save_exit_code, t_env **env_list,
		t_command **command_list)
{
	input = readline("Minishell > ");
	if (input == NULL)
		exit_cmd(*command_list, env_list, save_exit_code);
	if (input && *input)
		add_history(input);
	return (input);
}

void	process_commands(t_command **command_list, char *input,
		int *save_exit_code, t_env **env_list)
{
	*command_list = init_command(*save_exit_code, env_list);
	if (process_input(command_list, input, save_exit_code, env_list))
	{
		if ((*command_list)->error_file > 0 && (*command_list)->next == NULL)
			check_error_file(*command_list);
		else if (just_a_path(*command_list))
		{
			check_heredoc(*command_list);
			select_type(*command_list, env_list);
		}
	}
	*save_exit_code = last_exitcode(*command_list);
}

void	cleanup(t_command **command_list, char *input)
{
	free_command_list(command_list);
	free(input);
	input = NULL;
}

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
