/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lisambet <lisambet@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/17 13:05:26 by lvan-slu          #+#    #+#             */
/*   Updated: 2024/11/07 10:37:23 by lisambet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int		g_received_signal = 0;

int	is_executable(t_command *command)
{
	if (command)
	{
		if (command->args[0] || command->file > 0)
			return (1);
	}
	return (0);
}

void	free_env_node(t_env *node)
{
	if (node)
	{
		free(node->name);
		free(node->value);
		free(node);
	}
}

void	free_env_list(t_env *env_list)
{
	t_env	*tmp;

	while (env_list)
	{
		tmp = env_list;
		env_list = env_list->next;
		free_env_node(tmp);
	}
}

int	just_a_path(t_command *command)
{
	struct stat	path_stat;
	int			fd;

	while (command)
	{
		if (command->args[0][0] == '.' || command->args[0][0] == '/')
		{
			if (stat(command->args[0], &path_stat) == 0)
			{
				if (S_ISDIR(path_stat.st_mode))
				{
					ft_printf_error("bash: %s: Is a directory\n",
						command->args[0]);
					command->exit_code = 126;
					return (0);
				}
				else if (access(command->args[0], X_OK) != 0)
				{
					ft_printf_error("bash: %s: Permission denied\n",
						command->args[0]);
					command->exit_code = 126;
					return (0);
				}
			}
			else
			{
				fd = open(command->args[0], O_RDONLY);
				if (fd < 0)
				{
					ft_printf_error("bash: %s: No such file or directory\n",
						command->args[0]);
					command->exit_code = 127;
					return (0);
				}
				close(fd);
			}
		}
		command = command->next;
	}
	return (1);
}

int	main(int argc, char **argv, char **envp)
{
	int			status;
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
	setup_signal_handling();
	while (1)
	{
		status = 1;
		input = readline("Minishell > ");
		if (!input)
			exit_cmd(command_list, &env_list, save_exit_code);
		handle_received_signal(&save_exit_code);
		if (input && *input)
			add_history(input);
		process_input(&command_list, &env_list, input, &save_exit_code);
		if (command_list->error_file > 0 && command_list->next == NULL)
			check_error_file(command_list);
		else
		{
			if (is_executable(command_list) && save_exit_code != 256)
			{
				if ((command_list->args[0] != NULL)
					&& (command_list->args[0][0] == '.'
						|| command_list->args[0][0] == '/'))
					status = just_a_path(command_list);
				if (status == 1)
				{
					check_heredoc(command_list);
					select_type(command_list, &env_list);
				}
			}
		}
		save_exit_code = last_exitcode(command_list);
		free_command_list(command_list);
		command_list = NULL;
		free(input);
		input = NULL;
	}
	free_env_list(env_list);
	return (0);
}
