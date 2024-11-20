/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lvan-slu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/14 16:24:27 by lvan-slu          #+#    #+#             */
/*   Updated: 2024/11/14 16:24:36 by lvan-slu         ###   ########.fr       */
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

void	read_heredoc(int pipe_fd_write, const char *end_of_input)
{
	char	*input;

	g_received_signal = 0;
	while (1)
	{
		input = readline("> ");
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
		write(pipe_fd_write, input, ft_strlen(input));
		write(pipe_fd_write, "\n", 1);
		free(input);
	}
	close(pipe_fd_write);
}
