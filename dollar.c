#include "minishell.h"

int check_builtins(t_command *command, t_env **env_list)
{
    int status = 0;

    if (ft_strcmp(command->args[0], "$") == 0)
        status = 1;
    else if (ft_strcmp(command->args[0], "exit") == 0)
        status = 1;
    else if (ft_strcmp(command->args[0], "echo") == 0 && ft_strlen(command->args[0]) == 4)
        status = 1;
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
    return status;
}


void	process_dollar(const char **input, char *buffer, int *buf_index, char quote_type)
{
	char	*dollar;
	int		temp_index;

	dollar = search_dollar(*input, quote_type);
	temp_index = 0;
	if (dollar)
	{
		while (dollar[temp_index])
			buffer[(*buf_index)++] = dollar[temp_index++];
		free(dollar);
	}
	else if (**input == '$' && !ft_wholeisalpha((*input)[1]))
		buffer[(*buf_index)++] = *(*input)++;
	else
		buffer[*buf_index] = '\0';
	while (**input && **input != ' ' && **input != '"' && **input != '\'')
		(*input)++;
}

void	handle_dollar_sign(const char **input, char *buffer, int *buf_index)
{
	char	quote_type;

	quote_type = '\0';
	if (*(*input + 1) == '\'' || *(*input + 1) == '"')
	{
		//printf("string %s\n", *input);
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
		process_dollar(input, buffer, buf_index, quote_type);
}

int	handle_dollar(const char *input, int *i, char *result, int *result_index)
{
	char	*env_value;

	(*i)++;
	if (input[*i] == '?')
		return (0);
	env_value = get_env_value(input, i);
	//printf("dd %s\n", env_value);
	if (env_value)
	{
		while (*env_value)
			result[(*result_index)++] = *env_value++;
		return (1);
	}
	else if (!env_value && (input[*i] == '"' || input[*i] == '\'') && !ft_wholeisalpha(input[*i + 1]))
	{
		//printf("dd %s\n", env_value);
		(*i)++;
		return (0);
	}
	else
	{
		(*i)++;
		return (0);
	}
}
char	*search_dollar(const char *input, char quote_type)
{
	char	result[1024];
	int		result_index;
	int		i;
	(void)quote_type;

	result_index = 0;
	i = 0;
	//printf("string %s\n", input);
	while (ft_isprint(input[i]))
	{
		if (input[i] == '"' || input[i] == '\'')
		{
			i++;
			continue;
		}
		if (input[i] == '$' && ft_wholeisalpha(input[i + 1]))
			handle_dollar(input, &i, result, &result_index);
		else //if (quote_type == '"')
			result[result_index++] = input[i++];
		//else 
		//	break;
	}
	result[result_index] = '\0';
	return (ft_strdup(result));
}
