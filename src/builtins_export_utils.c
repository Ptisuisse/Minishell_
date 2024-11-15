/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_export_utils.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lvan-slu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/17 13:04:49 by lvan-slu          #+#    #+#             */
/*   Updated: 2024/10/17 13:04:51 by lvan-slu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	append_node(t_env *env_list, char *value)
{
	char	*new_value;

	if (!env_list->value)
		new_value = ft_strdup(value);
	else
		new_value = ft_strjoin(env_list->value, value);
	free(env_list->value);
	free(value);
	env_list->value = new_value;
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

int	check_each_argument(t_command *command, int *equal, int j)
{
	if (command->args[1][j] == '=' || (command->args[1][j] == '+'
			&& command->args[1][j + 1] == '='))
	{
		*equal = 1;
		if (command->args[1][j] == '+' && command->args[1][j + 1] == '=')
			return (1);
		if (command->args[1][j + 1] == '\0' || command->args[1][j] == '\0')
			return (1);
		else if (command->args[1][j + 1] != '=')
		{
			if (j > 0 && !ft_isalnum(command->args[1][j + 1]))
				return (print_error(command));
		}
	}
	else if (command->args[1][j] == '-' || (!ft_isalnum(command->args[1][j]))
		|| command->args[1][j] == '_')
	{
		if (*equal != 1)
			return (print_error(command));
	}
	return (1);
}

int	ft_is_valid(t_command *command)
{
	int	i;
	int	j;
	int	equal;

	i = 1;
	equal = 0;
	while (command->args[i])
	{
		if (command->args[i][0] == '+' || ft_isdigit(command->args[i][0]))
			return (print_error(command));
		if (command->args[i][0] == '_' || command->args[i][0] == '=')
		{
			if (command->args[i][1] == '\0')
				return (print_error(command));
		}
		j = 0;
		while (command->args[i][j])
		{
			if (!check_each_argument(command, &equal, j))
				return (0);
			j++;
		}
		i++;
	}
	return (1);
}
