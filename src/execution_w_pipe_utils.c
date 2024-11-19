/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_w_pipe_utils.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lvan-slu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 15:10:03 by lvan-slu          #+#    #+#             */
/*   Updated: 2024/11/15 15:10:06 by lvan-slu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	wait_for_commands(t_command *commands)
{
	int			status;
	int			last_exit_code;
	t_command	*cmd;

	cmd = commands;
	while (commands)
	{
		waitpid(commands->pid, &status, 0);
		if (WIFEXITED(status))
			last_exit_code = WEXITSTATUS(status);
		commands->exit_code = last_exit_code;
		commands = commands->next;
	}
	commands = cmd;
	check_error_file(cmd);
}

void	setup_pipes(t_command *commands)
{
	if (commands->next != NULL)
	{
		if (pipe(commands->pipe) == -1)
		{
			ft_printf_error("pipe error\n");
			exit_cmd(commands, &commands->env, EXIT_FAILURE);
		}
	}
}

void	process_child_pipe(t_command *commands, t_env **env_list,
		int *prev_pipe_fd)
{
	close(commands->save_in);
	close(commands->save_out);
	if (*prev_pipe_fd != -1)
	{
		dup2(*prev_pipe_fd, STDIN_FILENO);
		close(*prev_pipe_fd);
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
