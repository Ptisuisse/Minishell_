/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_var.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lisambet <lisambet@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/25 17:05:25 by lisambet          #+#    #+#             */
/*   Updated: 2024/09/30 20:58:15 by lisambet         ###   ########.fr       */
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

int	handle_double_quotes(const char **input, char *buffer, int *buf_index)
{
	char	quote_type;
	char	*dollar;
	int		temp_index;
	int		in_quotes;

	in_quotes = 1;
	quote_type = **input;
	(*input)++;
	//if (**input == quote_type)
	//	(*input)++;
	printf("dollar %s\n", *input);
	dollar = search_dollar(*input);
	printf("dollar 2 %c\n", **input);
	temp_index = 0;
	while (**input)
	{
	if (dollar)
	{
		printf("dollar 3 %s\n", dollar);
		while (dollar[temp_index]) // && dollar[temp_index] != quote_type
			{
				printf("dollar 4 %c\n", dollar[temp_index]);
				if (dollar[temp_index] == '$' && dollar[temp_index + 1] == quote_type)
					{
						// if (dollar[temp_index] == '$' && dollar[temp_index + 1] == quote_type && dollar[temp_index + 2] == '\0')
						// 	{
						// 		printf("dollar AAAA %s\n", dollar);
						// 		buffer[(*buf_index)++] = dollar[temp_index++];
						// 		break;
						// 	}
						while (**input && **input != quote_type)
							(*input)++;	
						//git addprintf("dollar 5 %s\n", dollar);
						in_quotes = 0;
						return in_quotes;}
				buffer[(*buf_index)++] = dollar[temp_index++];
				}
		//printf("dollar 3 %s\n", dollar);
		free(dollar);
		while (**input && **input != quote_type)
       		(*input)++;
		//printf("input %c\n", **input);
		in_quotes = 0;
    // Stop further processing after dollar is handled
    	return in_quotes;
	}
	else
	{
		printf("dollar 5 %c\n", dollar[temp_index]);
		while (**input && **input != quote_type)
			buffer[(*buf_index)++] = *(*input)++;
		return in_quotes;
		break;
	}
	while (**input && **input != quote_type)
		(*input)++;	
	if 	(**input && **input == quote_type)
	{		
		if 	(**input && (**input == '"' || **input == '\''))
			(*input)++;
	}
	}
	//printf("ALAL %s\n", *input);
	//if (**input != quote_type)
	//	(*input)++;
	return in_quotes;
}

int	handle_single_quotes(const char **input, char *buffer, int *buf_index)
{
	char	quote_type;
	int		in_quotes;

	in_quotes = 1;
	quote_type = '\'';
	(*input)++;
	while (**input && **input != quote_type)
		buffer[(*buf_index)++] = *(*input)++;
	return in_quotes;
}
