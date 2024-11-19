/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dollar_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lisambet <lisambet@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/19 13:20:34 by lisambet          #+#    #+#             */
/*   Updated: 2024/11/19 13:30:22 by lisambet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*process_character(const char *input, int *i, int *result_index, t_command **command_list)
{
	char	*temp;

	if (input[*i] == '$')
	{
		temp = process_dollar_case(input, i, result_index, command_list);
	}
	else
	{
		temp = handle_remaining_chars(input, i, result_index);
	}
	return (temp);
}

char	*concatenate_results(char *result, char *temp)
{
	char	*new_result;

	if (!result)
		return (temp);
	new_result = ft_strjoin(result, temp);
	free(result);
	free(temp);
	return (new_result);
}

char	*process_dollar_case(const char *input, int *i, int *result_index, t_command **command_list)
{
	char	*env_key;
	char	*env_value;

	(void)result_index;
	(*i)++;
	env_key = extract_env_key(input, i);
	if (!env_key || env_key[0] == '\0')
		return handle_special_cases(input, i);

	env_value = find_env_value(env_key, (*command_list)->env);
	free(env_key);

	if (!env_value)
		return (ft_strdup("")); 
		
	return (ft_strdup(env_value));
}