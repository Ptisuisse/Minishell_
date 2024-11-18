/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_export_sort.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lvan-slu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 09:59:44 by lvan-slu          #+#    #+#             */
/*   Updated: 2024/11/15 09:59:47 by lvan-slu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

char	*append_value(char *old_value, char *new_value)
{
	char	*result;

	result = ft_strjoin(old_value, new_value);
	free(old_value);
	free(new_value);
	return (result);
}

void	update_value(t_env *tmp, char *value, int append)
{
	if (append)
		tmp->value = append_value(tmp->value, value);
	else
	{
		free(tmp->value);
		tmp->value = value;
	}
}
