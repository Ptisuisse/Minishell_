/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection_manager.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lvan-slu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/06 11:12:28 by lvan-slu          #+#    #+#             */
/*   Updated: 2024/11/06 11:53:31 by lvan-slu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	redirect_management(t_command *command, t_env **env_list)
{
	if (command->append_file != NULL)
		append_file(command);
	if (command->input_file != NULL)
		redirect_input(command, env_list);
	if (command->output_file != NULL)
		redirect_output(command, env_list);
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
		if (!access(filename, F_OK))
			commands->exit_code = 1;
		else
			commands->exit_code = 1;
		return (1);
	}
	dup2(fd, STDIN_FILENO);
	close(fd);
	put_into_args(commands);
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
		if (!access(filename, F_OK))
			command->exit_code = 1;
		else
			command->exit_code = 1;
		return (1);
	}
	dup2(fd, STDOUT_FILENO);
	close(fd);
	put_into_args(command);
	choose_command(command, env_list);
	return (0);
}

void	put_into_args(t_command *commands)
{
	int	i;

	i = 0;
	while (commands->args[i])
	{
		if (!commands->args[i] && commands->input_file)
			commands->args[i] = ft_strdup(commands->input_file);
		if (!commands->args[i] && commands->heredoc_file)
			commands->args[i] = ft_strdup(commands->append_file);
		i++;
	}
}

void	process_input(t_command **command_list, t_env **env_list, char *input,
		int *save_exit_code)
{
	(void)env_list;
	if (!input)
		return ;
	if (parse_command_line(input, command_list, *save_exit_code))
	{
		(*command_list)->exit_code = 0;
		*save_exit_code = 256;
	}
}
