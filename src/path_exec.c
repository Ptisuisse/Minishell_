/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path_exec.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lvan-slu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 15:31:19 by lvan-slu          #+#    #+#             */
/*   Updated: 2024/11/15 15:31:20 by lvan-slu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	check_path(char *pathname)
{
	if (!ft_strncmp(pathname, "/bin/", 5))
		return (1);
	return (0);
}

void	join_path(t_command *command, char **path)
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
			command->args[0] = full_path;
			break ;
		}
		free(full_path);
		full_path = NULL;
		i++;
	}
}

char	*find_path(t_env **env_list, t_command *command)
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
	join_path(command, path);
	free_split(path);
	return (command->args[0]);
}
