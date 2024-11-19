/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dollar.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lisambet <lisambet@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/17 13:05:01 by lvan-slu          #+#    #+#             */
/*   Updated: 2024/11/19 13:44:22 by lisambet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"


void	process_dollar(const char **input, char *buffer, int *buf_index,
		t_command **command_list)
{
	char	*dollar;
	int		temp_index;

	dollar = search_dollar(*input, command_list);
	temp_index = 0;
	if (dollar)
	{
		while (dollar[temp_index])
			buffer[(*buf_index)++] = dollar[temp_index++];
		free(dollar);
	}
	else if (**input == '$' && !ft_isalpha((*input)[1]))
		buffer[(*buf_index)++] = *(*input)++;
	else
		buffer[*buf_index] = '\0';
	while (**input && **input != ' ' && **input != '"' && **input != '\''
		&& **input != '=')
		(*input)++;
	// free(dollar);
}

void	handle_dollar_sign(const char **input, char *buffer, int *buf_index,
		t_command **command_list)
{
	char	quote_type;

	quote_type = '\0';
	if (*(*input + 1) == '\'' || *(*input + 1) == '"')
	{
		quote_type = *(*input + 1);
		(*input)++;
	}
	if (quote_type != '\0')
	{
		(*input)++;
		while (**input && **input != quote_type)
			buffer[(*buf_index)++] = *(*input)++;
		if (**input == quote_type)
			(*input)++;
	}
	else
		process_dollar(input, buffer, buf_index, command_list);
}


char	*handle_special_cases(const char *input, int *i)
{
	if ((input[*i] == '"' || input[*i] == '\'') && !ft_isalpha(input[*i + 1]))
	{
		(*i)++;
		return (NULL);
	}
	(*i)++;
	return (NULL);
}

char	*handle_dollar(const char *input, int *i, int *result_index, t_command **cmd)
{
	char		*env_key;
	char		*env_value;

	(void)result_index;
	(*i)++;
	env_key = extract_env_key(input, i);
	if (!env_key || env_key[0] == '\0')
		return handle_special_cases(input, i);

	env_value = find_env_value(env_key, (*cmd)->env);
	free(env_key);

	if (!env_value)
		return (ft_strdup("")); 
		
	return (ft_strdup(env_value));
}


char	*search_dollar(const char *input, t_command **command_list)
{
	char	*result;
	char	*temp;
	int		result_index;
	int		i;

	result = NULL;
	temp = NULL;
	result_index = 0;
	i = 0;
	while (ft_isprint(input[i]) && input[i] != ' ' && input[i] != '=')
	{
		skip_quotes(input, &i);
		temp = process_character(input, &i, &result_index, command_list);
		if (!temp)
			return (NULL);
		result = concatenate_results(result, temp);
		if (!result)
			return (NULL);
	}
	if (result)
		result[result_index] = '\0';
	return (ft_strdup(result));
}



