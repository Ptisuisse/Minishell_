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

// 1 ecriture  0 lecture pipe

#include "minishell.h"

void	append_child(t_command *command, t_env **env_list)
{
	int pipe_fd;

	pipe_fd = open(command->append_outfile, O_CREAT | O_WRONLY | O_APPEND, 0644);
	if (pipe_fd < 0)
	{
		perror("Failed to open append_outfile");
		return;
	}
	dup2(pipe_fd, STDOUT_FILENO);
	choose_command(command, env_list);
	close(pipe_fd);
	exit(EXIT_SUCCESS);
}

void	append_parent(t_command *command, int *pipe_fd, int stdout_backup)
{
	(void)command;
	close(pipe_fd[WRITE_END]);
	wait(NULL);
	close(pipe_fd[READ_END]);
	dup2(stdout_backup, STDOUT_FILENO);
	close(stdout_backup);
}

void	append_file(t_command *command, t_env **env_list)
{
	int		pipe_fd[2];
	int		pid;
	int		stdout_backup;

	stdout_backup = dup(STDOUT_FILENO);
	if (pipe(pipe_fd) < 0)
	{
		perror("pipe error");
		return;
	}
	pid = fork();
	if (pid == -1)
	{
		perror("fork error");
		return ;
	}
	if (pid == 0)
		append_child(command, env_list);
	else
		append_parent(command, pipe_fd, stdout_backup);
	return ;
}
