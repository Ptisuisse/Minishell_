/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lisambet <lisambet@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/17 13:06:00 by lvan-slu          #+#    #+#             */
/*   Updated: 2024/11/05 15:48:16 by lisambet         ###   ########.fr       */
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

void free_command_list(t_command *command_list)
{
    t_command *temp;
    int i = 0;

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
    temp->file = 0;
	temp->status = 0;
	temp->error_file = 0;
	temp->exit_code = 0;
	temp->output_file = NULL;
	temp->input_file = NULL;
	temp->append_file = NULL;
	temp->heredoc_file= NULL;
	temp->next = NULL;
	temp->prev = NULL;
    command_list = command_list->next;
    free(temp);
    }
}
