/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_free.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lvan-slu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/14 16:42:37 by lvan-slu          #+#    #+#             */
/*   Updated: 2024/11/14 16:42:40 by lvan-slu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_env_node(t_env *node)
{
	if (node)
	{
		free(node->name);
		free(node->value);
		free(node);
	}
}

void	free_env_list(t_env **env_list)
{
	t_env	*tmp;

	while ((*env_list))
	{
		tmp = (*env_list)->next;
		if ((*env_list)->name)
			free((*env_list)->name);
		if ((*env_list)->value)
			free((*env_list)->value);
		free((*env_list));
		(*env_list) = tmp;
	}
	(*env_list) = NULL;
}
