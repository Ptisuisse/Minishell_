/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_export.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lvan-slu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/17 13:04:44 by lvan-slu          #+#    #+#             */
/*   Updated: 2024/10/17 13:04:45 by lvan-slu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_env	*add_new_var(char *name, char *value, t_env *env_list)
{
	t_env	*new_node;

	new_node = malloc(sizeof(t_env));
	if (!new_node)
	{
		perror("malloc");
		free(name);
		free(value);
		return (NULL);
	}
	new_node->name = name;
	new_node->value = value;
	new_node->next = env_list;
	return (new_node);
}

int	update_existing_var(t_env *tmp, char *name, char *value, int append)
{
	while (tmp)
	{
		if (ft_strcmp(tmp->name, name) == 0
			&& ft_strlen(tmp->name) == ft_strlen(name))
		{
			if (append)
				append_node(tmp, value);
			else
			{
				free(tmp->value);
				tmp->value = value;
			}
			free(name);
			return (1);
		}
		tmp = tmp->next;
	}
	return (0);
}

char	*env_name_substr(char *equal_sign, char *arg)
{
	char	*name;

	name = NULL;
	if (equal_sign > arg && *(equal_sign - 1) == '+')
		name = ft_substr(arg, 0, equal_sign - arg - 1);
	else
		name = ft_substr(arg, 0, equal_sign - arg);
	return (name);
}

t_env	*export_args(char *arg, t_env *env_list)
{
	t_env	*tmp;
	char	*name;
	char	*value;
	char	*equal_sign;
	int		append;

	name = NULL;
	tmp = env_list;
	equal_sign = ft_strchr(arg, '=');
	append = 0;
	if (equal_sign)
	{
		if (equal_sign > arg && *(equal_sign - 1) == '+')
			append = 1;
		name = env_name_substr(equal_sign, arg);
		value = ft_strdup(equal_sign + 1);
	}
	else
	{
		name = ft_strdup(arg);
		value = NULL;
	}
	if (update_existing_var(tmp, name, value, append))
		return (env_list);
	return (add_new_var(name, value, env_list));
}

t_env	*export_cmd(t_env *env_list, t_command *command)
{
	t_env	*head;

	head = env_list;
	if (command->args[1] != NULL)
	{
		if (!ft_is_valid(command))
			return (head);
		env_list = export_args(command->args[1], env_list);
		if (env_list != NULL)
			head = env_list;
	}
	else
		print_variables(&head);
	return (head);
}
