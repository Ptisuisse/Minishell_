/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_env.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lvan-slu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/17 13:04:36 by lvan-slu          #+#    #+#             */
/*   Updated: 2024/10/17 13:04:38 by lvan-slu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_env	*create_env_node(char *env_entry)
{
	t_env	*new;
	char	*equal_sign;

	new = malloc(sizeof(t_env));
	if (!new)
		return (NULL);
	new->next = NULL;
	equal_sign = ft_strchr(env_entry, '=');
	if (equal_sign)
	{
		new->name = ft_substr(env_entry, 0, equal_sign - env_entry);
		equal_sign++;
		new->value = ft_strdup(equal_sign);
	}
	else
	{
		new->name = ft_strdup(env_entry);
		new->value = NULL;
	}
	return (new);
}

void	create_env_list(char **envp, t_env **env_list)
{
	int		i;
	t_env	*new;
	t_env	*tmp;

	i = 0;
	while (envp[i])
	{
		new = create_env_node(envp[i]);
		if (!new)
			return ;
		if (i == 0)
			*env_list = new;
		else
		{
			tmp = *env_list;
			while (tmp->next)
				tmp = tmp->next;
			tmp->next = new;
		}
		i++;
	}
}

void	env_cmd(t_env *env_list)
{
	t_env	*head;

	head = env_list;
	while (env_list)
	{
		if (!env_list->value)
			env_list = env_list->next;
		else
		{
			printf("%s=%s\n", env_list->name, env_list->value);
			env_list = env_list->next;
		}
	}
	env_list = head;
}


void	unset_cmd(t_command *command, t_env **env_list)
{
	t_env	*prev = NULL;
	t_env	*current = *env_list;
	t_env	*temp;

	if (command->args[1] == NULL)
	{
		command->exit_code = 0;
		return;
	}
	while (current)
	{
		if (ft_strcmp(current->name, command->args[1]) == 0)
		{
			if (prev == NULL)
				*env_list = current->next;
			else
				prev->next = current->next;
			temp = current;
			current = current->next;
			free(temp->name);
			free(temp->value);
			free(temp);
			command->exit_code = 0;
			return;
		}
		prev = current;
		current = current->next;
	}
	command->exit_code = 1;
}

// void	unset_cmd(t_command *command, t_env **env_list)
// {
// 	t_env	*prev;

// 	prev = NULL;
// 	if (command->args[1] == NULL)
// 	{
// 		command->exit_code = 0;
// 		return ;
// 	}
// 	while (env_list)
// 	{
// 		if (ft_strcmp((*env_list)->name, command->args[1]) == 0)
// 		{
// 			if (prev == NULL)
// 				env_list = (*env_list)->next;
// 			else
// 				prev->next = (*env_list)->next;
// 		}
// 		prev = env_list;
// 		env_list = (*env_list)->next;
// 	}
// 	env_list = prev;
// }
