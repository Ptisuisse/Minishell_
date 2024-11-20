/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_manager.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lisambet <lisambet@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/17 13:04:55 by lvan-slu          #+#    #+#             */
/*   Updated: 2024/11/20 13:19:41 by lisambet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	close_backup_fd(int save_out, int save_in)
{
	dup2(save_out, 1);
	close(save_out);
	dup2(save_in, 0);
	close(save_in);
}

void	save_stdin_stdout(t_command *command, int save_in, int save_out)
{
	t_command	*head;

	head = command;
	while (command)
	{
		command->save_in = save_in;
		command->save_out = save_out;
		command = command->next;
	}
	command = head;
}

void	select_type(t_command *command, t_env **list)
{
	int	save_in;
	int	save_out;

	save_in = dup(STDIN_FILENO);
	save_out = dup(STDOUT_FILENO);
	command->save_in = save_in;
	command->save_out = save_out;
	if (command->next)
	{
		save_stdin_stdout(command, save_in, save_out);
		commands_pipe_manager(command, list);
	}
	else
	{
		if (command->file > 0 && command->heredoc_file == NULL)
			redirect_management(command, list);
		else
			choose_command(command, list);
		check_error_file(command);
	}
	close_backup_fd(save_out, save_in);
	if (command->heredoc_file != NULL)
		remove(".heredoc");
}

void	start_builtins(t_command *command, t_env **env_list)
{
	if (!(ft_strcmp(command->args[0], "exit")))
		exit_cmd(command, env_list, 0);
	else if (ft_strcmp(command->args[0], "echo") == 0
		|| (ft_strcmp(command->args[0], "$") == 0))
		echo_cmd(command);
	else if (!(ft_strcmp(command->args[0], "cd")))
		cd_cmd(command, *env_list);
	else if (!(ft_strcmp(command->args[0], "pwd")))
		pwd_cmd(command);
	else if (!(ft_strcmp(command->args[0], "env")))
		env_cmd(*env_list);
	else if (!(ft_strcmp(command->args[0], "export")))
		*env_list = export_cmd(*env_list, command);
	else if (!(ft_strcmp(command->args[0], "unset")))
		unset_cmd(command, env_list);
}

void	check_heredoc(t_command *command)
{
	t_command	*head;

	head = command;
	while (command)
	{
		if (command->heredoc_file)
			heredoc(command);
		command = command->next;
	}
}
