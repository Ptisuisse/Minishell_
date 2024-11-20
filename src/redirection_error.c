/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection_error.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lvan-slu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/06 11:11:31 by lvan-slu          #+#    #+#             */
/*   Updated: 2024/11/06 11:11:33 by lvan-slu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	parsing_error_inputfile(t_command *commands, char *buffer)
{
	struct stat	filestat;

	commands->input_file = ft_strdup(buffer);
	if (access(commands->input_file, F_OK) == -1)
	{
		commands->error_file = 1;
		commands->error_message = ft_strdup(" No such file or directory");
		return (2);
	}
	else if (access(commands->input_file, R_OK) == -1)
	{
		commands->error_file = 1;
		commands->error_message = ft_strdup(" Permission denied");
		return (1);
	}
	else if (stat(commands->input_file, &filestat) == 0 && S_ISDIR(filestat.st_mode))
	{
		commands->error_file = 1;
		commands->error_message = ft_strdup(" is a directory");
		return (1);
	}
	return (0);
}

int	parsing_error_appendfile(t_command *commands)
{
	char	*filename;

	filename = commands->append_file;
	if (check_file(filename, commands) > 0)
	{
		if (commands->input_file != NULL)
			commands->input_file = NULL;
		if (commands->output_file != NULL)
			commands->output_file = NULL;
		return (1);
	}
	return (0);
}

int	parsing_error_outputfile(t_command *commands, char *buffer)
{
	char	*filename;
	int		fd;

	commands->output_file = ft_strdup(buffer);
	fd = open(commands->output_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	close(fd);
	filename = commands->output_file;
	if (check_file(filename, commands) > 0)
	{
		if (commands->append_file != NULL)
			commands->append_file = NULL;
		if (commands->input_file != NULL)
			commands->input_file = NULL;
		return (1);
	}
	return (0);
}
