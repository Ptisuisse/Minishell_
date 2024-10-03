
#include "minishell.h"

int    check_builtins(char **command, t_data *data)
{
    int    status;

    status = 0;
    if (ft_strcmp(command[0], "$") == 0)
        status = 1;
    else if (!(ft_strcmp(command[0], "exit")))
        status = 1;
    else if (!(ft_strcmp(command[0], "echo")))
        status = 1;
    else if (!(ft_strcmp(command[0], "cd")))
        status = 1;
    else if (!(ft_strcmp(command[0], "export")))
        status = 1;
    else if (!(ft_strcmp(command[0], "unset")))
        status = 1;
    else if (!(ft_strcmp(command[0], "env")))
        status = 1;
    else if (!(ft_strcmp(command[0], "pwd")))
        status = 1;
    else if (!(ft_strcmp(command[0], "clear")))
        clear_cmd();
    if (status == 1)
        start_builtins(command, data);
    return (status);
}

void	process_dollar(const char **input, char *buffer, int *buf_index, char quote_type)
{
	char	*dollar;
	int		temp_index;

	dollar = search_dollar(*input);
	temp_index = 0;
	printf("dollar 2 %c\n", **input);
	if (dollar)
	{
		printf("dollar 3 %s\n", dollar);
		while (dollar[temp_index] && dollar[temp_index] != quote_type)
			buffer[(*buf_index)++] = dollar[temp_index++];
		free(dollar);
	}
	else if (!dollar && *(*(input + 1)))
		buffer[(*buf_index)++] = *(*input)++;
		//buffer[*buf_index] = '\0';
	else
		buffer[*buf_index] = '\0';
	//if (**input == quote_type)
	//	(*input)++;
	while (**input)
		(*input)++;
}

void	handle_dollar_sign(const char **input, char *buffer, int *buf_index)
{
	char	quote_type = '\0';

	if (*(*input + 1) == '\'' || *(*input + 1) == '"')
		quote_type = *(*input + 1);
	//if (**input == quote_type)
	//	(*input)++;
	process_dollar(input, buffer, buf_index, quote_type);
}

int	handle_dollar(const char *input, int *i, char *result, int *result_index)
{
	char	*env_value;

	//printf("dollar = %c\n", input[*i]);
	(*i)++;
	if (input[*i] == '?')
		return 0; // handle $? as needed
	//printf("dollar 2 = %c\n", input[*i]);
	env_value = get_env_value(input, i);
	//printf("dollar 3 = %c\n", input[*i]);
	//printf("dollar 4 = %s\n", env_value);
	if (env_value)
	{
		while (*env_value)
			result[(*result_index)++] = *env_value++;
		return 1;
	}
	else if (!env_value && (input[*i] == '"' || input[*i] == '\'') && !ft_isalpha(input[*i + 1]))
		{
			//printf("dollar 5 %c\n", input[*i]);
			(*i)++;
			//result[(*result_index)++] = '$';
			return 0;
			}
	else // if (ft_isprint(input[*i]))
		{
			//printf("dollar %c\n", input[*i]);
			//result[(*result_index)++] = '$';
			(*i)++;
			return 0;
			}
}

char	*search_dollar(const char *input)
{
	char	result[1024];
	int		result_index;
	int		i;
	int	flag; 

	result_index = 0;
	i = 0;
	while (ft_isprint(input[i]))
	{
		while (input[i] == '"' || input[i] == '\'')
		{
			i++;  // Stop if we hit a quote but do not return NULL
		}
		if (input[i] == '$' && ft_isalpha(input[i + 1]))
			{
				printf("dollar 6 %s\n", input);
				flag = handle_dollar(input, &i, result, &result_index);}
		else if (input[i] == '$' && !flag)
			{
				printf("dollar 7 %s\n", input);
				result[result_index] = '$';
				i++;
			}
		else if (input[i] == '"' || input[i] == '\'')
		{
			break;  // Stop if we hit a quote but do not return NULL
		}
		else
			{
				if (input[i] == '$' && ft_isalpha(input[i + 1]))
							{
								printf("dollar AAAA %s\n", input);
								result[result_index++] = input[i++];
								break;
							}
				printf("dollar 8 %s\n", input);
				result[result_index++] = input[i++];}
	}
	printf("dollar 9 %s\n", result);
	result[result_index] = '\0';
	return (ft_strdup(result));
}
