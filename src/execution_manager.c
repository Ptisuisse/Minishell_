/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_manager.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lvan-slu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/17 13:04:55 by lvan-slu          #+#    #+#             */
/*   Updated: 2024/11/06 11:52:05 by lvan-slu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	select_type(t_command *command, t_env **list)
{
	int	save_in;
	int	save_out;

	save_in = dup(STDIN_FILENO);
	save_out = dup(STDOUT_FILENO);
	if (command->next)
		commands_pipe_manager(command, list);
	else
	{
		if (command->file > 0 && command->heredoc_file == NULL)
			redirect_management(command, list);
		else
			choose_command(command, list);
		check_error_file(command);
	}
	if (command->heredoc_file != NULL)
		remove(".heredoc");
	dup2(save_out, STDOUT_FILENO);
	dup2(save_in, STDIN_FILENO);
}

void	start_builtins(t_command *command, t_env **env_list)
{
	if (!(ft_strcmp(command->args[0], "exit")))
		exit_cmd(command, env_list);
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
	command = head;
}
// void	handle_child_process(t_command *commands)
//{
//	if (commands->prev == NULL && commands->next)
//	{
//		close(commands->pipe[0]);
//		dup2(commands->pipe[1], STDOUT_FILENO);
//		close(commands->pipe[1]);
//	}
//	else if (commands->next && commands->prev)
//	{
//		close(commands->prev->pipe[1]);
//		dup2(commands->prev->pipe[0], STDIN_FILENO);
//		close(commands->prev->pipe[0]);
//		close(commands->pipe[0]);
//		dup2(commands->pipe[1], STDOUT_FILENO);
//		close(commands->pipe[1]);
//	}
//	else if (commands->prev && commands->next == NULL)
//	{
//		dup2(commands->prev->pipe[0], STDIN_FILENO);
//		close(commands->prev->pipe[1]);
//	}
//}

// void	setup_pipes(t_command *commands)
//{
//	if (commands->next)
//		pipe(commands->pipe);
//	commands->pid = fork();
//}
