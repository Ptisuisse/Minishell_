/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lisambet <lisambet@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/17 15:11:58 by lvan-slu          #+#    #+#             */
/*   Updated: 2024/11/19 17:30:50 by lisambet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	heredoc_parent(t_command *command, int *pipe_fd)
{
	int	heredoc_fd;

	(void)command;
	close(pipe_fd[WRITE_END]);
	write_to_heredoc(pipe_fd[READ_END]);
	close(pipe_fd[READ_END]);
	heredoc_fd = open(".heredoc", O_RDONLY);
	if (heredoc_fd == -1)
	{
		perror("Failed to reopen .heredoc");
		return ;
	}
	if (dup2(heredoc_fd, STDIN_FILENO) == -1)
		perror("dup2 error");
	close(heredoc_fd);
	if (!ft_strcmp(command->args[0], "cat") || !ft_strcmp(command->args[0],
			"grep"))
		put_into_args(command);
}

void	heredoc_child(t_command *command, int *pipe_fd)
{
	close(pipe_fd[READ_END]);
	read_heredoc(pipe_fd[WRITE_END], command->heredoc_file);
	close(pipe_fd[WRITE_END]);
	exit(EXIT_SUCCESS);
}

void	heredoc(t_command *command)
{
	int	pipe_fd[2];
	int	pid;
	int	stdin_backup;

	stdin_backup = dup(STDIN_FILENO);
	if (pipe(pipe_fd) == -1)
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
		heredoc_child(command, pipe_fd);
	else
	{
		waitpid(pid, &command->status, 0);
		heredoc_parent(command, pipe_fd);
		dup2(stdin_backup, STDIN_FILENO);
		close(stdin_backup);
	}
}

int	handle_heredoc(const char **input, t_command *cmd, char *buffer)
{
	int	buf_index;

	buf_index = 0;
	(*input)++;
	while (**input == ' ')
		(*input)++;
	parse_argument(input, buffer, &buf_index, &cmd);
	cmd->heredoc_file = ft_strdup(buffer);
	return (0);
}
