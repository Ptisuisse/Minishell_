/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dollar_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lisambet <lisambet@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/19 13:20:34 by lisambet          #+#    #+#             */
/*   Updated: 2024/11/19 17:33:19 by lisambet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*process_character(const char *input, int *i, int *result_index,
		t_command **command_list)
{
	char	*temp;

	temp = NULL;
	if (input[*i] == '$' && input[(*i + 1)] == '?')
	{
		temp = replace_by_exit_code(temp, result_index, command_list);
		(*result_index) += ft_strlen(temp);
		*i += 2;
	}
	else if (input[*i] == '$' && ft_isalnum(input[(*i + 1)]))
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

char	*process_dollar_case(const char *input, int *i, int *result_index,
		t_command **command_list)
{
	char	*env_key;
	char	*env_value;

	(void)result_index;
	(void)*i;
	env_key = extract_env_key(input);
	if (!env_key || env_key[0] == '\0')
		return (handle_special_cases(input, i));
	env_value = find_env_value(env_key, (*command_list)->env);
	free(env_key);
	if (!env_value)
		return (ft_strdup(""));
	(*result_index) += ft_strlen(env_value);
	return (ft_strdup(env_value));
}
