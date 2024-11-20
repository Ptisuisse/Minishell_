/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   append_file.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lvan-slu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/21 15:05:50 by lvan-slu          #+#    #+#             */
/*   Updated: 2024/10/21 15:05:51 by lvan-slu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	append_child(t_command *command, t_env **env_list, int *pipe)
{
	int	pipe_fd;

	close(command->save_in);
	close(command->save_out);
	close(pipe[0]);
	close(pipe[1]);
	pipe_fd = open(command->append_file, O_CREAT | O_WRONLY | O_APPEND, 0644);
	if (pipe_fd < 0)
	{
		if (!access(command->append_file, F_OK))
		{
			command->exit_code = 1;
			return ;
		}
		else
		{
			command->exit_code = 1;
			return ;
		}
	}
	dup2(pipe_fd, STDOUT_FILENO);
	close(pipe_fd);
	choose_command_pipe(command, env_list);
	return ;
}

void	append_file(t_command *command, t_env **env_list)
{
	int	pipe_fd[2];
	int	pid;

	if (pipe(pipe_fd) < 0)
	{
		perror("pipe error");
		return ;
	}
	pid = fork();
	if (pid == -1)
	{
		perror("fork error");
		return ;
	}
	if (pid == 0)
	{
		append_child(command, env_list, pipe_fd);
		exit(EXIT_SUCCESS);
	}
	else
	{
		waitpid(pid, &command->status, 0);
		if (WIFEXITED(command->status))
			command->exit_code = WEXITSTATUS(command->status);
	}
	close(command->save_in);
	close(command->save_out);
	close(pipe_fd[0]);
	close(pipe_fd[1]);
}
