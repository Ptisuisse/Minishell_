/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_files.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lvan-slu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/14 16:04:05 by lvan-slu          #+#    #+#             */
/*   Updated: 2024/11/14 16:04:07 by lvan-slu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	check_directory(const char *path)
{
	struct stat	path_stat;

	if (stat(path, &path_stat) != 0)
		return (0);
	return (S_ISDIR(path_stat.st_mode));
}

int	check_if_directory(const char *filename, t_command *commands)
{
	struct stat	filestat;

	if (stat(filename, &filestat) == 0 && S_ISDIR(filestat.st_mode))
	{
		commands->error_file = 1;
		commands->error_message = ft_strdup(" is a directory");
		return (2);
	}
	return (0);
}

int	check_file_directory(const char *filename, t_command *commands)
{
	char	*dirname;
	char	*last_slash;
	int		result;

	result = 0;
	dirname = ft_strdup((char *)filename);
	last_slash = ft_strrchr(dirname, '/');
	if (last_slash != NULL)
	{
		*last_slash = '\0';
		if (!check_directory(dirname))
		{
			commands->error_file = 1;
			commands->error_message = ft_strdup(" No such file or directory");
			result = 1;
		}
	}
	free(dirname);
	return (result);
}

int	check_file(const char *filename, t_command *commands)
{
	int	result;

	result = check_if_directory(filename, commands);
	if (result != 0)
		return (result);
	result = check_file_directory(filename, commands);
	if (result != 0)
		return (result);
	if (access(filename, F_OK) == -1)
		return (0);
	if (access(filename, W_OK) == -1)
	{
		commands->error_file = 1;
		commands->error_message = ft_strdup(" Permission denied");
		return (1);
	}
	return (0);
}
