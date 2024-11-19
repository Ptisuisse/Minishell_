/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dollar.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lisambet <lisambet@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/17 13:05:01 by lvan-slu          #+#    #+#             */
/*   Updated: 2024/11/07 10:30:15 by lisambet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	check_builtins(t_command *command, t_env **env_list)
{
	int	status;

	status = 0;
	if (ft_strcmp(command->args[0], "") == 0)
		status = 0;
	else if (ft_strcmp(command->args[0], "exit") == 0
		&& ft_strlen(command->args[0]) == 4)
		status = 1;
	else if (ft_strcmp(command->args[0], "echo") == 0
		&& ft_strlen(command->args[0]) == 4)
		status = 1;
	else if (ft_strcmp(command->args[0], "cd") == 0
		&& ft_strlen(command->args[0]) == 2)
		status = 1;
	else if (ft_strcmp(command->args[0], "export") == 0
		&& ft_strlen(command->args[0]) == 6)
		status = 1;
	else if (ft_strcmp(command->args[0], "unset") == 0
		&& ft_strlen(command->args[0]) == 5)
		status = 1;
	else if (ft_strcmp(command->args[0], "env") == 0
		&& ft_strlen(command->args[0]) == 3)
		status = 1;
	else if (ft_strcmp(command->args[0], "pwd") == 0
		&& ft_strlen(command->args[0]) == 3)
		status = 1;
	if (status == 1)
		start_builtins(command, env_list);
	return (status);
}

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

char	*handle_dollar(const char *input, int *i, int *result_index,
		t_command **cmd)
{
	char	*env_value;
	int		j;
	t_command	*head;

	(void)result_index;
	head = *cmd;
	(*i)++;
	j = 0;
	env_value = malloc(sizeof(char *) * (ft_strlen(input)));
	while (input[j])
	{
		env_value[j] = input[*i];
		j++;
		(*i)++;
	}
	env_value[j] = '\0';
	while ((*cmd)->env)
	{
		if (ft_strcmp((*cmd)->env->name, env_value) == 0)
		{
			env_value = (*cmd)->env->value;
			j = 1;
			break ;
		}
		else
			j = -1;
		(*cmd)->env = (*cmd)->env->next;
	}
	*cmd = head;
	if (j == -1)
		env_value[0] = '\0';
	if (env_value)
		return (env_value);
	else if (!env_value && (input[*i] == '"' || input[*i] == '\'')
			&& !ft_isalpha(input[*i + 1]))
	{
		(*i)++;
		return (NULL);
	}
	else
	{
		(*i)++;
		return (NULL);
	}
}

char	*search_dollar(const char *input, t_command **command_list)
{
	char	*result;
	int		result_index;
	int		i;

	result_index = 0;
	i = 0;
	while (ft_isprint(input[i]) && input[i] != ' ' && input[i] != '=')
	{
		if (input[i] == '"' || input[i] == '\'')
		{
			i++;
			continue ;
		}
		if (input[i] == '$' && (input[i + 1] == '?'))
		{
			result = replace_by_exit_code(result, &result_index, command_list);
			result_index++;
			i += 2;
			continue ;
		}
		if (input[i] == '$' && ft_isalnum(input[i + 1]))
		{
			result = handle_dollar(input, &i, &result_index, command_list);
			if (result == NULL)
				return (NULL);
			result_index += ft_strlen(result);
		}
		else
		{
			result = ft_strdup((char *)input);
			result_index += ft_strlen(result);
			i += 2;
		}
	}
	result[result_index] = '\0';
	return (ft_strdup(result));
}
