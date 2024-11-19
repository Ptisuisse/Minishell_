/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection_manager.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lisambet <lisambet@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/06 11:12:28 by lvan-slu          #+#    #+#             */
/*   Updated: 2024/11/19 10:21:03 by lisambet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	redirect_management(t_command *command, t_env **env_list)
{
	if (command->error_file == 0 && command->file > 1)
		multiple_redirection(command, env_list);
	else if (command->error_file == 0 && command->file == 1)
	{
		if (command->input_file != NULL)
			redirect_input(command, env_list);
		if (command->output_file != NULL)
			redirect_output(command, env_list);
		if (command->append_file != NULL)
			append_file(command, env_list);
	}
}

int	redirect_input(t_command *commands, t_env **env_list)
{
	char	*filename;
	int		fd;

	(void)env_list;
	filename = commands->input_file;
	fd = open(filename, O_RDONLY);
	if (fd < 0)
	{
		commands->exit_code = 1;
		return (1);
	}
	dup2(fd, STDIN_FILENO);
	close(fd);
	choose_command(commands, env_list);
	return (0);
}

int	redirect_output(t_command *command, t_env **env_list)
{
	char	*filename;
	int		fd;

	filename = command->output_file;
	fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd < 0)
	{
		command->exit_code = 1;
		return (1);
	}
	dup2(fd, STDOUT_FILENO);
	close(fd);
	choose_command(command, env_list);
	return (0);
}

void	put_into_args(t_command *commands)
{
	int	i;

	i = 0;
	while (commands->args[i])
		i++;
	if (!commands->args[i] && commands->heredoc_file)
		commands->args[i] = ft_strdup(".heredoc");
}

int	process_input(t_command **command_list, char *input, int *save_exit_code,
		t_env **env_list)
{
	if (!input || !(*input))
		return (0);
	if (ft_strlen(input) > 1024 && ft_strncmp(input, "echo", 4) != 0)
	{
		input = ft_memmove(input, input, 10);
		input[10] = '\0';
	}
	if (parse_command_line(input, command_list, *save_exit_code, env_list))
	{
		if (!ft_strcmp(input, "$EMPTY"))
			(*command_list)->exit_code = 0;
		else
			(*command_list)->exit_code = 2;
		*save_exit_code = 256;
		return (0);
	}
	return (1);
}
