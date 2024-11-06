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

int	print_error(t_command *command)
{
	command->exit_code = 1;
	if (command->args[0])
		ft_printf("Minishell: export: `%s': not a valid identifier\n",
			command->args[0]);
	else
		ft_printf("Minishell: export: not a valid identifier\n");
	return (0);
}

int	check_each_argument(t_command *command, int *equal)
{
	int	j;

	j = 0;
	while (command->args[1][j])
	{
		if (command->args[1][0] == '_')
		{
			if (command->args[1][1] == '\0')
				return (print_error(command));
		}
		else if (command->args[1][j] == '=')
		{
			*equal = 1;
			if (!ft_isalnum(command->args[1][j - 1]))
				return (print_error(command));
		}
		else if (command->args[1][j] == '-' || (!ft_isalnum(command->args[1][j])
				|| ft_isdigit(command->args[1][0]))
			|| command->args[1][j] == '_')
		{
			if (*equal != 1)
				return (print_error(command));
		}
		j++;
	}
	return (1);
}

int	ft_is_valid(t_command *command)
{
	int	i;
	int	equal;

	i = 1;
	while (command->args[i])
	{
		equal = 0;
		if (!check_each_argument(command, &equal))
			return (0);
		i++;
	}
	return (1);
}

void	sorted_insert(t_env **head_ref, t_env *new_node)
{
	t_env	*current;

	if (*head_ref == NULL || ft_strcmp((*head_ref)->name, new_node->name) > 0)
	{
		new_node->next = *head_ref;
		*head_ref = new_node;
	}
	else
	{
		current = *head_ref;
		while (current->next != NULL && ft_strcmp(current->next->name,
				new_node->name) <= 0)
		{
			current = current->next;
		}
		new_node->next = current->next;
		current->next = new_node;
	}
}

void	sort_env_list(t_env **head_ref)
{
	t_env	*sorted;
	t_env	*current;
	t_env	*next;

	sorted = NULL;
	current = *head_ref;
	while (current != NULL)
	{
		next = current->next;
		sorted_insert(&sorted, current);
		current = next;
	}
	*head_ref = sorted;
}
