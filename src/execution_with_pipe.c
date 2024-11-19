/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_with_pipe.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lvan-slu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/06 11:25:47 by lvan-slu          #+#    #+#             */
/*   Updated: 2024/11/06 11:25:48 by lvan-slu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	choose_command_pipe(t_command *command, t_env **env_list)
{
	int	result;

	result = -1;
	if (check_builtins(command, env_list))
		result = 0;
	else
	{
		exec_pipe_command(command, env_list);
		result = 0;
	}
	return (result);
}

void	exec_pipe_command(t_command *command, t_env **env_list)
{
	char	*cmd;
	char	**envp;

	envp = create_envp(*env_list);
	cmd = ft_strdup(command->args[0]);
	if (!check_path(command->args[0]))
		cmd = find_path(env_list, command, cmd);
	if (execve(cmd, &command->args[0], envp) == -1)
	{
		command->exit_code = 127;
		ft_printf_error("%s: command not found\n", command->args[0]);
		exit(command->exit_code);
	}
	free(cmd);
}

void	handle_command(t_command *commands, t_env **env_list, int *prev_pipe_fd)
{
	setup_pipes(commands);
	commands->pid = fork();
	if (commands->pid == -1)
		perror("fork error");
	if (commands->pid == 0)
		process_child_pipe(commands, env_list, prev_pipe_fd);
	else if (commands->pid > 0)
	{
		if (*prev_pipe_fd != -1)
			close(*prev_pipe_fd);
		if (commands->next)
			close(commands->pipe[1]);
		*prev_pipe_fd = commands->pipe[0];
	}
}

void	commands_pipe_manager(t_command *commands, t_env **env_list)
{
	t_command	*cmd;
	int			prev_pipe_fd;

	prev_pipe_fd = -1;
	cmd = commands;
	while (commands)
	{
		if (commands->error_file > 0)
		{
			if (commands->next != NULL)
				commands = commands->next;
			else
				break ;
		}
		handle_command(commands, env_list, &prev_pipe_fd);
		commands = commands->next;
	}
	commands = cmd;
	// if (prev_pipe_fd != -1)
	//{
	//	close(prev_pipe_fd);
	//}
	wait_for_commands(cmd);
}
