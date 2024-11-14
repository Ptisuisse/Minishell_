/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lvan-slu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/06 11:26:50 by lvan-slu          #+#    #+#             */
/*   Updated: 2024/11/06 11:52:16 by lvan-slu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	check_path(char *pathname)
{
	if (!ft_strncmp(pathname, "/bin/", 5))
		return (1);
	return (0);
}

char	*find_path(t_env **env_list, char *pathname)
{
	t_env	*head;
	char	**path;
	char	*full_path;
	int		i;

	i = 0;
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
	while (path[i])
	{
		full_path = ft_strjoin(ft_strjoin(path[i], "/"), pathname);
		if (!(access(full_path, F_OK)))
		{
			pathname = full_path;
			break ;
		}
		free(full_path);
		i++;
	}
	free_split(path);
	return (pathname);
}

int	count_env_vars(t_env *env_list)
{
	int	count;

	count = 0;
	while (env_list)
	{
		count++;
		env_list = env_list->next;
	}
	return (count);
}

char	*strjoin_env(const char *s1, const char *s2, const char *s3)
{
	size_t	len;
	char	*result;

	len = ft_strlen(s1) + ft_strlen(s2) + ft_strlen(s3) + 1;
	result = malloc(sizeof(char) * len);
	if (result == NULL)
		return (NULL);
	ft_strcpy(result, s1);
	ft_strcat(result, s2);
	if (s3)
		ft_strcat(result, s3);
	return (result);
}

char	**create_envp(t_env *env_list)
{
	int		i;
	int		env_count;
	char	**envp;
	t_env	*current;

	i = 0;
	current = env_list;
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
