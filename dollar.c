/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dollar.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lvan-slu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/17 13:05:01 by lvan-slu          #+#    #+#             */
/*   Updated: 2024/10/17 13:05:02 by lvan-slu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	check_builtins(t_command *command, t_env **env_list)
{
    int    status;

    status = 0;
    if (ft_strcmp(command->args[0], "$") == 0)
        status = 1;
    else if (ft_strcmp(command->args[0], "exit") == 0)
        status = 1;
    //else if (ft_strcmp(command->args[0], "echo") == 0 && ft_strlen(command->args[0]) == 4)
    //    status = 1;
	else if (ft_strncmp(command->args[0], "echo", 5) == 0)
        status = 1;
	//else if (ft_strncmp(command->args[0], "cd", 3) == 0)
    //    status = 1;
    else if (ft_strcmp(command->args[0], "cd") == 0 && ft_strlen(command->args[0]) == 2)
        status = 1;
    else if (ft_strcmp(command->args[0], "export") == 0 && ft_strlen(command->args[0]) == 6)
        status = 1;
    else if (ft_strcmp(command->args[0], "unset") == 0 && ft_strlen(command->args[0]) == 5)
        status = 1;
    else if (ft_strcmp(command->args[0], "env") == 0 && ft_strlen(command->args[0]) == 3)
        status = 1;
    else if (ft_strcmp(command->args[0], "pwd") == 0 && ft_strlen(command->args[0]) == 3)
        status = 1;
    else if (ft_strcmp(command->args[0], "clear") == 0 && ft_strlen(command->args[0]) == 5)
		clear_cmd();
	if (status == 1)
        start_builtins(command, env_list);
    return (status);
}

void	process_dollar(const char **input, char *buffer, int *buf_index, t_command *command_list)
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
	while (**input && **input != ' ' && **input != '"' && **input != '\'' && **input != '=')
		(*input)++;
}

void	handle_dollar_sign(const char **input, char *buffer, int *buf_index, t_command *command_list)
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
	// while (**input)
	// 	input++;
}

int	handle_dollar(const char *input, int *i, char *result, int *result_index)
{
	char	*env_value;

	(*i)++;
	// if (input[*i] == '?')
	// 	result = replace_by_exit_code(result, result_index);
	env_value = get_env_value(input, i);
	if (env_value)
	{
		while (*env_value)
			result[(*result_index)++] = *env_value++;
		return (1);
	}
	else if (!env_value && (input[*i] == '"' || input[*i] == '\'') && !ft_isalpha(input[*i + 1]))
	{
		(*i)++;
		return (0);
	}
	else
	{
		(*i)++;
		return (0);
	}
}
char	*search_dollar(const char *input, t_command *command_list)
{
	char	result[1024];
	int		result_index;
	int		i;
//	char *exit_code;

	result_index = 0;
	i = 0;
	while (ft_isprint(input[i]) && input[i] != ' ' && input[i] != '=')
	{
		if (input[i] == '"' || input[i] == '\'')
		{
			i++;
			continue;
		}
		if (input[i] == '$' && (input[i + 1] == '?'))
		{
            replace_by_exit_code(result, &result_index, command_list);
            i += 2;
            continue;
        }
		if (input[i] == '$' && ft_isalnum(input[i + 1]))
			handle_dollar(input, &i, result, &result_index);
		else
			result[result_index++] = input[i++];
	}
	result[result_index] = '\0';
	return (ft_strdup(result));
}
