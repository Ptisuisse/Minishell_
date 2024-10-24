/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lvan-slu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/17 13:06:00 by lvan-slu          #+#    #+#             */
/*   Updated: 2024/10/17 13:06:01 by lvan-slu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

 void	printf_list(t_env *env_list)
{
	t_env	*tmp;

	tmp = env_list;
	while (tmp)
	{
		printf("%s=", tmp->name);
		printf("%s\n", tmp->value);
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

int    ft_strcmp(const char *s1, const char *s2)
{
    int            a;

    a = 0;
    while (s1[a] != '\0')
    {
        if (s1[a] != s2[a])
            return (s1[a] - s2[a]);
        a++;
    }
    return (0);
}

void	print_commands(t_command *command_list)
{
	int	i;
	int	j;

	i = 0;
	while (command_list)
	{
		printf("Command[%d]:\n", i);
		j = 0;
		while (command_list->args[j])
		{
			printf("  Arg[%d]: %s\n", j, command_list->args[j]);
			j++;
		}
		if (command_list->input_file)
			printf("  Input Redirection: %s\n", command_list->input_file);
		if (command_list->output_file)
			printf("  Output Redirection: %s\n", command_list->output_file);
		if (command_list->append_infile)
			printf("  Append Input Redirection: %s\n", command_list->append_infile);
		if (command_list->append_outfile)
			printf("  Append Output Redirection: %s\n", command_list->append_outfile);
		command_list = command_list->next;
		i++;
	}
}

void	free_commands(t_command *command_list)
{
	//int			j;
	t_command	*temp;

	while (command_list)
	{
		temp = command_list;
		temp->append_infd = 0;
		command_list->append_infd = 0;
		command_list = command_list->next;
	//	j = 0;
		// while (temp->args[j])
		// 	free(temp->args[j++]);
		free(temp->input_file);
		free(temp->output_file);
		free(temp);
	}
}
