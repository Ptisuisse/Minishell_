/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lisambet <lisambet@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/06 10:32:45 by lvan-slu          #+#    #+#             */
/*   Updated: 2024/11/07 11:06:06 by lisambet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// int	print_error_redirection(char *filename)
//{
//	int	fd;

//	fd = open(filename, O_RDONLY);
//	if (fd < 0)
//	{
//		if (!access(filename, F_OK))
//		{
//			ft_printf_error("bash: %s: Permission denied\n", filename);
//			return (1);
//		}
//		else
//			ft_printf_error("%s: No such file or directory\n", filename);
//	}
//	return (0);
//}

void	redirect_error(t_command *command, char *filename)
{
	while (command)
	{
		if (command->error_message != NULL)
		{
			ft_printf_error("bash : %s :%s\n", filename,
				command->error_message);
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
			ft_printf_error("bash : %s :%s\n", cmd->input_file,
				cmd->error_message);
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
