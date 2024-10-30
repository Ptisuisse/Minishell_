/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_management.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lvan-slu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/17 13:04:55 by lvan-slu          #+#    #+#             */
/*   Updated: 2024/10/17 13:04:56 by lvan-slu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// void	handle_parent_process(t_command *commands)
// {
// 	if (commands->prev == NULL && commands->next)
// 	{
// 		close(commands->pipe[1]);
// 		dup2(commands->pipe[0], 0);
// 		close(commands->pipe[0]);
// 	}
// 	else if (commands->next)
// 	{
// 		close(commands->prev->pipe[0]);
// 		dup2(commands->prev->pipe[1], STDOUT_FILENO);
// 		close(commands->prev->pipe[1]);
// 		//close(commands->pipe[1]);
// 		//dup2(commands->pipe[0], 0);
// 		//close(commands->pipe[0]);
// 	}
//}

void	handle_child_process(t_command *commands)
{
	if (commands->prev == NULL && commands->next)
	{
		close(commands->pipe[0]);
		dup2(commands->pipe[1], STDOUT_FILENO);
		close(commands->pipe[1]);
	}
	else if (commands->next && commands->prev)
	{
		close(commands->prev->pipe[1]);
		dup2(commands->prev->pipe[0], STDIN_FILENO);
		close(commands->prev->pipe[0]);
		close(commands->pipe[0]);
		dup2(commands->pipe[1], STDOUT_FILENO);
		close(commands->pipe[1]);
	}
	else if (commands->prev && commands->next == NULL)
	{
		dup2(commands->prev->pipe[0], STDIN_FILENO);
		close(commands->prev->pipe[1]);
	}
}

void	setup_pipes(t_command *commands)
{
	if (commands->next)
		pipe(commands->pipe);
	commands->pid = fork();
}

void	start_builtins(t_command *command, t_env **env_list)
{
	if (!(ft_strcmp(command->args[0], "exit")))
		exit_cmd(command);
	else if (!(ft_strcmp(command->args[0], "echo")))
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
		unset_cmd(command, *env_list);
}
