/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lisambet <lisambet@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/19 17:11:29 by lisambet          #+#    #+#             */
/*   Updated: 2024/11/19 17:22:30 by lisambet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_strcmp(const char *s1, const char *s2)
{
	int	a;

	a = 0;
	while (s1[a] != '\0')
	{
		if (s1[a] != s2[a])
			return (s1[a] - s2[a]);
		a++;
	}
	return (0);
}

void	free_command_args(t_command *command)
{
	int	i;

	i = 0;
	if (!command)
		return ;
	while (command->args[i])
	{
		free(command->args[i]);
		command->args[i] = NULL;
		i++;
	}
}

void	free_command_files(t_command *command)
{
	if (!command)
		return ;
	if (command->output_file)
	{
		free(command->output_file);
		command->output_file = NULL;
	}
	if (command->input_file)
	{
		free(command->input_file);
		command->input_file = NULL;
	}
	if (command->append_file)
	{
		free(command->append_file);
		command->append_file = NULL;
	}
	if (command->heredoc_file)
	{
		free(command->heredoc_file);
		command->heredoc_file = NULL;
	}
}

void	free_command_error_message(t_command *command)
{
	if (command->error_message)
	{
		free(command->error_message);
		command->error_message = NULL;
	}
}

void	free_command_list(t_command **command_list)
{
	t_command	*tmp;

	while (*command_list)
	{
		tmp = (*command_list)->next;
		free_command_args(*command_list);
		free_command_files(*command_list);
		free_command_error_message(*command_list);
		if ((*command_list)->error_message)
		{	
			free((*command_list)->error_message);
			(*command_list)->error_message = NULL;
		}
		//*command_list = NULL;
		free(*command_list);
		*command_list = tmp;
	}
	*command_list = NULL;
	free((*command_list));
}
