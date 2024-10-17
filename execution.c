/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lvan-slu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/17 13:05:18 by lvan-slu          #+#    #+#             */
/*   Updated: 2024/10/17 13:05:19 by lvan-slu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	check_path(char *pathname)
{
	if (!ft_strncmp(pathname, "/bin/", 5))
		return (1);
	return (0);
}

int	exec_command(char *pathname, char **args)
{
	int		pid;
	char	*path;

	if (!check_path(pathname))
		path = ft_strjoin("/bin/", pathname);
	else
		path = ft_strdup(pathname);
	pid = fork();
	if (pid == 0)
	{
		if (execve(path, args, NULL) == -1)
		{
			g_exit_code = 127;
			ft_printf("%s: command not found\n", pathname);
			return (0);
		}
	}
	free(path);
	//waitpid(pid, NULL, 0);
	return (1);
}

int	choose_command(t_command *command, t_env **env_list)
{
	int	result;

	result = -1;
	if (command->append_infd == 1)
		heredoc(command, env_list);
	if (check_builtins(command, env_list))
		result = 0;
	else
	{
		exec_command(command->args[0], command->args);
		result = 0;
	}
	return (result);
}

void	ft_process_wait(t_command *commands)
{
	int			status;
	t_command	*cmd;

	status = 0;
	cmd = commands;
	while (commands)
	{
		waitpid(commands->pid, &status, 0);
		commands = commands->next;
	}
	commands = cmd;
}
