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

int	update_existing_var(t_env *tmp, char *name, char *value, char *equal_sign,
		int append)
{
	char	*new_value;

	while (tmp)
	{
		if (ft_strcmp(tmp->name, name) == 0)
		{
			if (equal_sign)
			{
				if (append)
				{
					new_value = ft_strjoin(tmp->value, value);
					free(tmp->value);
					free(value);
					tmp->value = new_value;
				}
				else
				{
					free(tmp->value);
					tmp->value = value;
				}
			}
			free(name);
			return (1);
		}
		tmp = tmp->next;
	}
	return (0);
}

t_env	*export_args(char *arg, t_env *env_list)
{
	t_env	*tmp;
	char	*name;
	char	*value;
	char	*equal_sign;
	int		append;

	tmp = env_list;
	equal_sign = ft_strchr(arg, '=');
	append = 0;
	if (equal_sign && equal_sign > arg && *(equal_sign - 1) == '+')
	{
		append = 1;
		name = ft_substr(arg, 0, equal_sign - arg - 1);
		value = ft_strdup(equal_sign + 1);
	}
	else if (equal_sign)
	{
		name = ft_substr(arg, 0, equal_sign - arg);
		value = ft_strdup(equal_sign + 1);
	}
	else
	{
		name = ft_strdup(arg);
		value = NULL;
	}
	if (update_existing_var(tmp, name, value, equal_sign, append))
		return (env_list);
	return (add_new_var(name, value, env_list));
}

void	print_variables(t_env **head)
{
	t_env	*current;

	sort_env_list(head);
	current = *head;
	while (current)
	{
		if (current->name[0] == '_' && current->name[1] == '\0')
		{
			if (current->next)
				current = current->next;
			else
				break ;
		}
		if (current->value == NULL)
			ft_printf("declare -x %s\n", current->name);
		else
			ft_printf("declare -x %s=\"%s\"\n", current->name, current->value);
		current = current->next;
	}
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
