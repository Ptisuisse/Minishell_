/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_var.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lisambet <lisambet@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/17 13:05:06 by lvan-slu          #+#    #+#             */
/*   Updated: 2024/11/19 13:26:46 by lisambet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	skip_spaces(const char **input)
{
	while (**input == ' ')
		(*input)++;
}

void	skip_quotes(const char *input, int *i)
{
	while (input[*i] == '"' || input[*i] == '\'')
		(*i)++;
}
char	*handle_remaining_chars(const char *input, int *i, int *result_index)
{
	char	*result;

	result = ft_strdup((char *)input);
	*result_index += ft_strlen(result);
	(*i) += 2; // Skip non-dollar special cases
	return (result);
}

char	*extract_env_key(const char *input, int *i)
{
	char	*env_key;
	int		start = *i;
	int		length = 0;

	while (input[*i] && ft_isalnum(input[*i]))
	{
		length++;
		(*i)++;
	}

	env_key = malloc(sizeof(char) * (length + 1));
	if (!env_key)
		return (NULL);

	for (int j = 0; j < length; j++)
		env_key[j] = input[start + j];
	env_key[length] = '\0';
	return (env_key);
}

char	*find_env_value(const char *env_key, t_env *env)
{
	while (env)
	{
		if (ft_strcmp(env->name, env_key) == 0)
			return (env->value);
		env = env->next;
	}
	return (NULL);
}