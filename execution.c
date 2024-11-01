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

int    exec_pipe_command(t_command *command)
{
    char *path;

    if (!check_path(command->args[0]))
        path = ft_strjoin("/bin/", command->args[0]);
    else
        path = ft_strdup(command->args[0]);
    execve(path, command->args, NULL);
    ft_printf("%s: command not found\n", command->args[0]);
    free(path);
    exit(127);
}

char	*find_path(t_env **env_list, char *cmd)
{
	t_env	*head;
	char	**path;
	int	i;

	i = 0;
	head = *env_list;
	while(head->next)
	{
		if (ft_strcmp(head->name, "PATH") == 0 && ft_strlen(head->name) == 4)
			break ;
		head = head->next;
	}
	path = ft_split(head->value, ':');
	while (path[i])
	{
		path[i] = ft_strjoin(path[i], "/");
		path[i] = ft_strjoin(path[i], cmd);
		if (!(access(path[i], F_OK)))
			break ;
		i++;
	}
	return  path[i];
}

int    exec_command(t_command *command, t_env **env_list)
{
    char    *path;

    if (!check_path(command->args[0]))
		path = find_path(env_list, command);
    else
		path = ft_strdup(command->args[0]);
	command->pid = fork();
	if (command->pid == 0)
	{
		command->status = execve(path, command->args, NULL);
    	if (command->status == -1)
    	{
			command->exit_code = 127;
    	    ft_printf("%s: command not found\n", command->args[0]);
    	    return (0);
    	}
	}
    else
       ft_process_wait(command);
	free(path);
    return (1);
}

int	choose_command_pipe(t_command *command, t_env **env_list)
{
	int	result;

	result = -1;
	if (check_builtins(command, env_list))
		result = 0;
	else
	{
			exec_pipe_command(command);
		result = 0;
	}
	return (result);
}

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

void	ft_process_wait(t_command *commands)
{
	if (commands->pid != 0)
	{
		waitpid(commands->pid, &commands->status, 0);
	}
}
