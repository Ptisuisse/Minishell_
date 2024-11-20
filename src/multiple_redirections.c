/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   multiple_redirections.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lvan-slu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/14 16:14:59 by lvan-slu          #+#    #+#             */
/*   Updated: 2024/11/14 16:15:01 by lvan-slu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	multiple_append_child(t_command *command)
{
	int	pipe_fd;

	pipe_fd = open(command->append_file, O_CREAT | O_WRONLY | O_APPEND, 0644);
	if (pipe_fd < 0)
	{
		command->exit_code = 1;
		return ;
	}
	dup2(pipe_fd, STDOUT_FILENO);
	close(pipe_fd);
	return ;
}

void	multiple_append_file(t_command *command)
{
	multiple_append_child(command);
	free(command->append_file);
	command->append_file = NULL;
	return ;
}

int	multiple_redirection_input(t_command *command, t_env **env_list)
{
	char	*filename;
	int		fd;

	(void)env_list;
	filename = command->input_file;
	fd = open(filename, O_RDONLY);
	if (fd < 0)
	{
		command->exit_code = 1;
		return (1);
	}
	dup2(fd, STDIN_FILENO);
	close(fd);
	free(command->input_file);
	command->input_file = NULL;
	return (0);
}

int	multiple_redirection_output(t_command *command, t_env **env_list)
{
	char	*filename;
	int		fd;

	(void)env_list;
	filename = command->output_file;
	fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd < 0)
	{
		command->exit_code = 1;
		return (1);
	}
	dup2(fd, STDOUT_FILENO);
	close(fd);
	free(command->output_file);
	command->output_file = NULL;
	return (0);
}

void	multiple_redirection(t_command *command, t_env **env_list)
{
	while (command->file)
	{
		if (command->input_file != NULL)
			multiple_redirection_input(command, env_list);
		if (command->output_file != NULL && command->output == 4)
			multiple_redirection_output(command, env_list);
		if (command->append_file != NULL && command->output == 3)
			multiple_append_file(command);
		command->file--;
	}
	choose_command(command, env_list);
}
