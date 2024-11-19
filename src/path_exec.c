/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path_exec.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lisambet <lisambet@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 15:31:19 by lvan-slu          #+#    #+#             */
/*   Updated: 2024/11/19 17:34:52 by lisambet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	check_path(char *pathname)
{
	if (!ft_strncmp(pathname, "/bin/", 5))
		return (1);
	return (0);
}

char	*join_path(t_command *command, char **path, char *cmd)
{
	char	*temp_path;
	char	*full_path;
	int		i;

	i = 0;
	full_path = NULL;
	while (path[i])
	{
		temp_path = ft_strjoin(path[i], "/");
		full_path = ft_strjoin(temp_path, command->args[0]);
		free(temp_path);
		if (!(access(full_path, F_OK)))
		{
			cmd = ft_strdup(full_path);
			break ;
		}
		free(full_path);
		full_path = NULL;
		i++;
	}
	free(full_path);
	full_path = NULL;
	return (cmd);
}

char	*find_path(t_env **env_list, t_command *command, char *cmd)
{
	t_env	*head;
	char	**path;

	head = *env_list;
	while (head)
	{
		if (ft_strcmp(head->name, "PATH") == 0 && ft_strlen(head->name) == 4)
			break ;
		head = head->next;
	}
	if (!head)
		return (NULL);
	path = ft_split(head->value, ':');
	cmd = join_path(command, path, cmd);
	free_split(path);
	return (cmd);
}

int	process_command_path(const char *path, t_command *command)
{
	if (path[0] == '.' && path[1] != '/')
		return (1);
	return (validate_path(path, command));
}
