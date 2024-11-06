/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lvan-slu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/06 10:32:45 by lvan-slu          #+#    #+#             */
/*   Updated: 2024/11/06 10:32:47 by lvan-slu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	redirect_error(t_command *command)
{
	char	*filename;
	int		fd;

	while (command)
	{
		if (command->input_file != NULL)
			filename = command->input_file;
		else if (command->append_file != NULL)
			filename = command->append_file;
		else if (command->output_file != NULL)
			filename = command->output_file;
		else
			return ;
		fd = open(filename, O_RDONLY);
		if (fd < 0)
		{
			if (!access(filename, F_OK))
			{
				ft_printf("bash: %s: Permission denied\n", filename);
				return ;
			}
			else
				ft_printf("%s: No such file or directory\n", filename);
		}
		command = command->next;
	}
	return ;
}

void	check_error_file(t_command *cmd)
{
	t_command	*head;

	head = cmd;
	while (cmd)
	{
		if (cmd->error_file == 1)
		{
			redirect_error(cmd);
			cmd->exit_code = 1;
		}
		cmd = cmd->next;
	}
	cmd = head;
}

void	ft_close_fd(t_command *commands)
{
	while (commands)
	{
		close(commands->pipe[0]);
		close(commands->pipe[1]);
		commands = commands->next;
	}
}
