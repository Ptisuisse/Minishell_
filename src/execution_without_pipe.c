/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_without_pipe.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lisambet <lisambet@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/17 13:05:18 by lvan-slu          #+#    #+#             */
/*   Updated: 2024/11/20 13:20:19 by lisambet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	choose_command(t_command *command, t_env **env_list)
{
	int	result;

	result = -1;
	if (check_builtins(command, env_list))
		result = 0;
	else
	{
		exec_command(command, env_list);
		result = 0;
	}
	return (result);
}

void	exec_child_process(char *cmd, char **envp, t_command *command)
{
	close(command->save_in);
	close(command->save_out);
	if (execve(cmd, command->args, envp) == -1)
	{
		command->exit_code = 127;
		ft_printf_error("%s: command not found\n", command->args[0]);
		exit(command->exit_code);
	}
}

int	exec_command(t_command *command, t_env **env_list)
{
	char	*cmd;
	char	**envp;

	cmd = NULL;
	envp = create_envp(*env_list);
	if (!check_path(command->args[0]))
		cmd = find_path(env_list, command, cmd);
	else
		cmd = ft_strdup(command->args[0]);
	command->pid = fork();
	if (command->pid == -1)
		ft_printf_error("fork failed\n");
	if (command->pid == 0)
		exec_child_process(cmd, envp, command);
	else
		ft_process_wait(command);
	command->exit_code = WEXITSTATUS(command->status);
	free_split(envp);
	free(cmd);
	return (1);
}

void	ft_process_wait(t_command *command)
{
	if (command->pid != 0)
		waitpid(command->pid, &command->status, 0);
}
