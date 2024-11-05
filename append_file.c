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

void	append_child(t_command *command)
{
	int pipe_fd;

	pipe_fd = open(command->append_file, O_CREAT | O_WRONLY | O_APPEND, 0644);
	if (pipe_fd < 0)
	{
		if (!access(command->append_file, F_OK))
		{
			//ft_printf("bash: %s: Permission denied\n", command->append_file);
			command->exit_code = 1;
			return ;
		}
		else
		{
			//ft_printf("%s: No such file or directory\n", command->append_file);
			command->exit_code = 1;
			return ;
		}
	}
	dup2(pipe_fd, STDOUT_FILENO);
	choose_command(command, NULL);
	close(pipe_fd);
	return ;
}

//void	append_parent(t_command *command, int *pipe_fd, int stdout_backup)
//{
//	close(pipe_fd[READ_END]);
//	write_to_heredoc(pipe_fd[WRITE_END]);
//	close(pipe_fd[WRITE_END]);
//	int heredoc_fd = open(".heredoc", O_RDONLY);
//	if (heredoc_fd == -1)
//	{
//		perror("Failed to reopen .heredoc");
//		return;
//	}
//	if (dup2(heredoc_fd, STDOUT_FILENO) == -1)
//		perror("dup2 error");
//	close(heredoc_fd);
//	dup2(stdout_backup, STDOUT_FILENO);
//	close(stdout_backup);
//	command->args[READ_END] = ".heredoc";
//}

void	append_file(t_command *command)
{
	int		pipe_fd[2];
	int		pid;

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
		append_child(command);
	else
		waitpid(pid, NULL, 0);
	return ;
}
