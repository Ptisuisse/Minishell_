
#include "minishell.h"

int	check_builtins(char **command, t_data data)
{
	int	status;

	// printf("Je suis dans check_builtins\n");
	status = 0;
	if (ft_strcmp(command[0], "$"))
		status = 1;
	if (ft_strcmp(command[0], "exit"))
		status = 1;
	else if (ft_strcmp(command[0], "echo"))
		status = 1;
	else if (ft_strcmp(command[0], "cd"))
		status = 1;
	else if (ft_strcmp(command[0], "export"))
		status = 1;
	else if (ft_strcmp(command[0], "unset"))
		status = 1;
	else if (ft_strcmp(command[0], "env"))
		status = 1;
	else if (ft_strcmp(command[0], "pwd"))
		status = 1;
	if (status == 1)
		start_builtins(command, data);
	//else
	//	printf("No such command.\n");
	return (status);
}

void	process_dollar(const char **input, char *buffer, int *buf_index, char quote_type)
{
	char	*dollar;
	int		temp_index;

	dollar = search_dollar(*input);
	temp_index = 0;
	if (dollar)
	{
		while (dollar[temp_index] && dollar[temp_index] != quote_type)
			buffer[(*buf_index)++] = dollar[temp_index++];
		free(dollar);
	}
	else
		buffer[*buf_index] = '\0';
	if (**input == quote_type)
		(*input)++;
	while (**input)
		(*input)++;
}

void	handle_dollar_sign(const char **input, char *buffer, int *buf_index)
{
	char	quote_type = '\0';

	if (*(*input + 1) == '\'' || *(*input + 1) == '"')
		quote_type = *(*input + 1);
	if (**input == quote_type)
		(*input)++;
	process_dollar(input, buffer, buf_index, quote_type);
}

static void	handle_dollar(const char *input, int *i, char *result, int *result_index)
{
	char	*env_value;

	(*i)++;
	if (input[*i] == '?')
		return; // handle $? as needed
	env_value = get_env_value(input, i);
	if (env_value)
	{
		while (*env_value)
			result[(*result_index)++] = *env_value++;
	}
	else
		(*i)++;
}

char	*search_dollar(const char *input)
{
	char	result[1024];
	int		result_index;
	int		i;

	result_index = 0;
	i = 0;
	while (ft_isprint(input[i]))
	{
		if (input[i] == '$' && input[i + 1])
			handle_dollar(input, &i, result, &result_index);
		else
			result[result_index++] = input[i++];
	}
	result[result_index] = '\0';
	return (ft_strdup(result));
}