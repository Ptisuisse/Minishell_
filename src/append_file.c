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

int	check_append_file(t_command *commands)
{
	char		*filename;
	struct stat	filestat;

	filename = commands->append_file;
	if (access(filename, F_OK) == -1)
	{
		commands->error_file = 1;
		commands->error_message = ft_strdup(" No such file or directory");
		return (2);
	}
	else if (access(filename, R_OK) == -1)
	{
		commands->error_file = 1;
		commands->error_message = ft_strdup(" Permission denied");
		return (1);
	}
	else if (stat(filename, &filestat) == 0 && S_ISDIR(filestat.st_mode))
	{
		commands->error_file = 1;
		commands->error_message = ft_strdup(" is a directory");
		return (1);
	}
	return (0);
}

void	append_child(t_command *command, t_env **env_list)
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
	choose_command_pipe(command, env_list);
	close(pipe_fd);
	return ;
}

void	append_file(t_command *command, t_env **env_list)
{
	int	pipe_fd[2];
	int	pid;

	if (check_append_file(command))
		return ;
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
		append_child(command, env_list);
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
