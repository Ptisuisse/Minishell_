/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lvan-slu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/17 15:11:58 by lvan-slu          #+#    #+#             */
/*   Updated: 2024/10/17 15:11:59 by lvan-slu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	write_to_heredoc(int pipe_fd_read)
{
	int		heredoc_fd;
	char	buffer[1024];
	size_t	bytes_read;

	bytes_read = 0;
	heredoc_fd = open(".heredoc", O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (heredoc_fd == -1)
	{
		perror("Failed to open .heredoc");
		close(pipe_fd_read);
		return ;
	}
	bytes_read = read(pipe_fd_read, buffer, sizeof(buffer));
	while (bytes_read > 0)
	{
		write(heredoc_fd, buffer, bytes_read);
		bytes_read = read(pipe_fd_read, buffer, sizeof(buffer));
	}
	close(heredoc_fd);
}

void	heredoc_parent(t_command *command, int *pipe_fd)
{
	int	heredoc_fd;

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
	command->args[1] = ft_strdup(".heredoc");
}

void	read_heredoc(int pipe_fd_write, const char *end_of_input)
{
	char	*input;

	g_received_signal = 0;
	while (1)
	{
		setup_heredoc_signal_handling();
		input = readline("> ");
		if (g_received_signal == SIGINT)
		{
			printf("LALAL\n");
			close(pipe_fd_write);
			if (input)
				free(input);
			return ;
		}
		if (!input)
		{
			perror("readline error");
			close(pipe_fd_write);
			exit(EXIT_FAILURE);
		}
		if (strcmp(input, end_of_input) == 0)
		{
			free(input);
			break ;
		}
		write(pipe_fd_write, input, strlen(input));
		write(pipe_fd_write, "\n", 1);
		free(input);
	}
	close(pipe_fd_write);
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
	return ;
}
