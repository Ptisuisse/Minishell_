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


void free_split(char **split)
{
    int i = 0;
    if (!split)
        return;
    while (split[i]) 
	{
        free(split[i]);
        i++;
    }
    free(split);
}

void	find_path(t_env **env_list, t_command *command)
{
    t_env	*head;
    char	**path;
    char   *full_path;
    int	i;

    i = 0;
    head = *env_list;
    while (head)
    {
        if (ft_strcmp(head->name, "PATH") == 0 && ft_strlen(head->name) == 4)
            break ;
        head = head->next;
    }
    if (!head)
        return;
    path = ft_split(head->value, ':');
    while (path[i])
    {
        full_path = ft_strjoin(ft_strjoin(path[i], "/"), command->args[0]);
        if (!(access(full_path, F_OK)))
        {
            command->args[0] = full_path;
            break ;
        }
        free(full_path);
        i++;
    }
    free_split(path);
    return ;
}

int count_env_vars(t_env *env_list)
{
    int count = 0;
    while (env_list)
    {
        count++;
        env_list = env_list->next;
    }
    return count;
}

char *strjoin_env(const char *s1, const char *s2, const char *s3)
{
    size_t len;
    char *result;

	len = ft_strlen(s1) + ft_strlen(s2) + ft_strlen(s3) + 1;
	result = malloc(sizeof(char) * len);
    if (!result)
        return (NULL);
	ft_strcpy(result, s1);
    ft_strcat(result, s2);
    ft_strcat(result, s3);
    return (result);
}

char **create_envp(t_env *env_list)
{
    int     i = 0;
    int     env_count;
    char    **envp;
    t_env   *current = env_list;

	env_count = count_env_vars(env_list);
	envp = malloc(sizeof(char *) * (env_count + 1));
    if (!envp)
        return (NULL);
    while (current)
    {
        envp[i] = strjoin_env(current->name, "=", current->value);
        i++;
        current = current->next;
    }
    envp[i] = NULL;
    return (envp);
}

int exec_command(t_command *command, t_env **env_list)
{
    char    *cmd;
    char    **envp;
	
	envp = create_envp(*env_list);
	cmd = ft_strdup(command->args[0]);
    if (!check_path(command->args[0])) 
        find_path(env_list, command);
    command->pid = fork();
    if (command->pid == 0)
    {
        if (execve(command->args[0], command->args, envp) == -1)
        {
            command->exit_code = 127;
            ft_printf("%s: command not found\n", cmd);
            free(cmd);
            free(envp);
            exit(command->exit_code);
        }
    }
    else
        ft_process_wait(command);
    command->exit_code = WEXITSTATUS(command->status);
    free(cmd);
    for (int i = 0; envp[i] != NULL; i++)
        free(envp[i]);
    free(envp);
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
			exec_pipe_command(command, env_list);
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
		waitpid(commands->pid, &commands->status, 0);
}

int    exec_pipe_command(t_command *command, t_env **env_list)
{
    char    *cmd;
    char    **envp;
	
	envp = create_envp(*env_list);
	cmd = ft_strdup(command->args[0]);
    if (!check_path(command->args[0])) 
        find_path(env_list, command);
    if (execve(command->args[0], command->args, envp) == -1)
    {
        command->exit_code = 127;
        ft_printf("%s: command not found\n", cmd);
        free(cmd);
        free(envp);
        exit(command->exit_code);
    }
    command->exit_code = WEXITSTATUS(command->status);
    free(cmd);
    for (int i = 0; envp[i] != NULL; i++)
        free(envp[i]);
    free(envp);
    return (1);
}