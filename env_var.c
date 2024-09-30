/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_var.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lisambet <lisambet@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/25 17:05:25 by lisambet          #+#    #+#             */
/*   Updated: 2024/09/30 09:27:07 by lisambet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	skip_spaces(const char **input)
{
	while (**input == ' ')
		(*input)++;
}

char	*get_env_value(const char *input, int *i)
{
	char	temp[256];
	int		temp_index;
	char	*env_value;

	temp_index = 0;
	while (input[*i] && (ft_isalpha(input[*i]) || input[*i] == '_'))
		temp[temp_index++] = input[(*i)++];
	temp[temp_index] = '\0';
	env_value = getenv(temp);
	return (env_value);
}

void	handle_double_quotes(const char **input, char *buffer, int *buf_index)
{
	char	quote_type;
	char	*dollar;
	int		temp_index;

	quote_type = **input;
	(*input)++;
	if (**input == quote_type)
		(*input)++;
	dollar = search_dollar(*input);
	temp_index = 0;
	if (dollar)
	{
		while (dollar[temp_index] && dollar[temp_index] != quote_type)
			buffer[(*buf_index)++] = dollar[temp_index++];
		free(dollar);
	}
	else
	{
		while (**input && **input != quote_type)
			buffer[(*buf_index)++] = *(*input)++;
	}
	while (**input && **input != quote_type)
		(*input)++;
}

void	handle_single_quotes(const char **input, char *buffer, int *buf_index)
{
	char	quote_type;

	quote_type = '\'';
	(*input)++;
	while (**input && **input != quote_type)
		buffer[(*buf_index)++] = *(*input)++;
}
