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

void	append_child(t_command *command)
{
	int	pipe_fd;

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
	choose_command(command, NULL);
	close(pipe_fd);
	return ;
}

void	append_file(t_command *command)
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
		append_child(command);
		exit(EXIT_SUCCESS);
	}
	else
	{
		waitpid(pid, &command->status, 0);
		if (WIFEXITED(command->status))
			command->exit_code = WEXITSTATUS(command->status);
	}
	return ;
}
