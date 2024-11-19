/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   paths.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lisambet <lisambet@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/19 16:55:03 by lisambet          #+#    #+#             */
/*   Updated: 2024/11/19 17:31:31 by lisambet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	handle_is_directory(const char *path, t_command *command)
{
	ft_printf_error("bash: %s: Is a directory\n", path);
	command->exit_code = 126;
	return (0);
}

static int	handle_permission_denied(const char *path, t_command *command)
{
	ft_printf_error("bash: %s: Permission denied\n", path);
	command->exit_code = 126;
	return (0);
}

static int	handle_no_such_file(const char *path, t_command *command)
{
	ft_printf_error("bash: %s: No such file or directory\n", path);
	command->exit_code = 127;
	return (0);
}

int	validate_path(const char *path, t_command *command)
{
	struct stat	path_stat;
	int			fd;

	if (stat(path, &path_stat) == 0)
	{
		if (S_ISDIR(path_stat.st_mode))
			return (handle_is_directory(path, command));
		if (access(path, X_OK) != 0)
			return (handle_permission_denied(path, command));
	}
	else
	{
		fd = open(path, O_RDONLY);
		if (fd < 0)
			return (handle_no_such_file(path, command));
		close(fd);
	}
	return (1);
}

int	just_a_path(t_command *command)
{
	t_command	*current;
	int			status;

	current = command;
	status = 1;
	while (current)
	{
		if (current->args[0] && (current->args[0][0] == '.'
			|| current->args[0][0] == '/'))
		{
			if (!process_command_path(current->args[0], current))
				status = 0;
		}
		current = current->next;
	}
	return (status);
}
