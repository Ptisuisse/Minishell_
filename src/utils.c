/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lisambet <lisambet@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/17 13:06:00 by lvan-slu          #+#    #+#             */
/*   Updated: 2024/11/07 11:06:33 by lisambet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	print_error(t_command *command)
{
	command->exit_code = 1;
	if (command->args[0])
		ft_printf_error("Minishell: export: `%s': not a valid identifier\n",
			command->args[1]);
	else
		ft_printf_error("Minishell: export: not a valid identifier\n");
	return (0);
}

void	free_split(char **split)
{
	int	i;

	i = 0;
	if (!split)
		return ;
	while (split[i])
	{
		free(split[i]);
		i++;
	}
	free(split);
}

void	printf_list(t_env *env_list)
{
	t_env	*tmp;

	tmp = env_list;
	while (tmp)
	{
		ft_printf("%s=", tmp->name);
		ft_printf("%s\n", tmp->value);
		tmp = tmp->next;
	}
}

char	*ft_strcpy(char *dst, const char *src)
{
	size_t	i;

	i = 0;
	while (src[i])
	{
		dst[i] = src[i];
		i++;
	}
	dst[i] = '\0';
	return (dst);
}

char	*ft_strcat(char *dest, const char *src)
{
	size_t	i;
	size_t	j;

	i = ft_strlen(dest);
	j = 0;
	while (src[j])
	{
		dest[i] = src[j];
		dest++;
		src++;
	}
	dest[i] = '\0';
	return (dest);
}

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

void	free_command_list(t_command *command_list)
{
	t_command	*temp;
	int			i;

	i = 0;
	if (!command_list)
	{
		while (command_list)
		{
			temp = command_list;
			while (temp->args[i])
			{
				if (temp->args[i])
				{
					free(temp->args[i]);
					temp->args[i] = NULL;
					i++;
				}
			}
			if (temp->output_file)
			{
				free(temp->output_file);
				temp->output_file = NULL;
			}
			if (temp->input_file)
			{
				free(temp->input_file);
				temp->input_file = NULL;
			}
			if (temp->append_file)
			{
				free(temp->append_file);
				temp->append_file = NULL;
			}
			if (temp->error_message)
			{
				free(temp->error_message);
				temp->error_message = NULL;
			}
			temp->file = 0;
			temp->status = 0;
			temp->error_file = 0;
			temp->output_file = NULL;
			temp->input_file = NULL;
			temp->append_file = NULL;
			temp->heredoc_file = NULL;
			temp->error_message = NULL;
			temp->next = NULL;
			temp->prev = NULL;
			command_list = command_list->next;
			free(temp);
		}
	}
}
