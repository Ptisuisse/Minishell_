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

int	print_error(char *arg)
{
	g_exit_code = 1;
	if (arg)
		ft_printf("Minishell: export: `%s': not a valid identifier\n", arg);
	else
		ft_printf("Minishell: export: not a valid identifier\n");
	return (0);
}

int	check_each_argument(char *arg, int *equal)
{
	int	j;

	j = 0;
	while (arg[j])
	{
		if (arg[0] == '_')
		{
			if (arg[1] == '\0')
				return (print_error(arg));
		}
		else if (arg[j] == '=')
		{
			*equal = 1;
			if (!ft_isalnum(arg[j - 1]))
				return (print_error(arg));
		}
		else if ((!ft_isalnum(arg[j]) || ft_isdigit(arg[0])) || arg[j] == '_')
		{
			if (*equal != 1)
				return (print_error(NULL));
		}
		j++;
	}
	return (1);
}

int	ft_is_valid(char **arg)
{
	int	i;
	int	equal;

	i = 1;
	while (arg[i])
	{
		equal = 0;
		if (!check_each_argument(arg[i], &equal))
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
		while (current->next != NULL
			&& ft_strcmp(current->next->name, new_node->name) <= 0)
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
