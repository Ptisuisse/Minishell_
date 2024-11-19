/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lisambet <lisambet@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/17 13:06:00 by lvan-slu          #+#    #+#             */
/*   Updated: 2024/11/19 10:58:56 by lisambet         ###   ########.fr       */
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

static void	free_command_args(t_command *command)
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

static void	free_command_files(t_command *command)
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

static void	free_command_error_message(t_command *command)
{
	if (command && command->error_message)
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
		free(*command_list);
		*command_list = tmp;
	}
	*command_list = NULL;
	free((*command_list));
}
