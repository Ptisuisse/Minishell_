/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_exec.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lvan-slu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/06 11:26:50 by lvan-slu          #+#    #+#             */
/*   Updated: 2024/11/15 15:32:20 by lvan-slu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
