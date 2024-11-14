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

int	exec_pipe_command(t_command *command, t_env **env_list)
{
	char	*cmd;
	char	**envp;

	envp = create_envp(*env_list);
	cmd = ft_strdup(command->args[0]);
	if (!check_path(command->args[0]))
		cmd = find_path(env_list, command);
	if (execve(cmd, command->args, envp) == -1)
	{
		command->exit_code = 127;
		ft_printf_error("%s: command not found\n", command->args[0]);
		free(cmd);
		free_split(envp);
		exit(command->exit_code);
	}
	command->exit_code = WEXITSTATUS(command->status);
	free(cmd);
	free_split(envp);
	return (1);
}

void	commands_pipe_manager(t_command *commands, t_env **env_list)
{
	t_command	*cmd;
	int			prev_pipe_fd;
	int			status;
	int			last_exit_code;

	cmd = commands;
	prev_pipe_fd = -1;
	while (commands)
	{
		if (commands->error_file > 0)
		{
			if (commands->next != NULL)
				commands = commands->next;
			else
				break ;
		}
		if (commands->next && pipe(commands->pipe) == -1)
		{
			perror("pipe error");
			exit(EXIT_FAILURE);
		}
		commands->pid = fork();
		if (commands->pid == 0)
		{
			if (prev_pipe_fd != -1)
			{
				dup2(prev_pipe_fd, STDIN_FILENO);
				close(prev_pipe_fd);
			}
			if (commands->next)
			{
				close(commands->pipe[0]);
				dup2(commands->pipe[1], STDOUT_FILENO);
				close(commands->pipe[1]);
			}
			if (commands->file > 0)
				redirect_management(commands, env_list);
			else
				choose_command_pipe(commands, env_list);
			exit(EXIT_SUCCESS);
		}
		else if (commands->pid > 0)
		{
			if (prev_pipe_fd != -1)
				close(prev_pipe_fd);
			if (commands->next)
				close(commands->pipe[1]);
			prev_pipe_fd = commands->pipe[0];
		}
		else
		{
			perror("fork error");
			exit(EXIT_FAILURE);
		}
		commands = commands->next;
	}
	commands = cmd;
	while (commands)
	{
		waitpid(commands->pid, &status, 0);
		if (WIFEXITED(status))
			last_exit_code = WEXITSTATUS(status);
		commands->exit_code = last_exit_code;
		commands = commands->next;
	}
	commands = cmd;
	check_error_file(commands);
}
