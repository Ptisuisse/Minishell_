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
	while (input[*i] && (ft_wholeisalpha(input[*i]) || input[*i] == '_'))
		temp[temp_index++] = input[(*i)++];
	temp[temp_index] = '\0';
	env_value = getenv(temp);
	//printf("aaa %s\n", env_value);
	return (env_value);
}
int	handle_double_quotes(const char **input, char *buffer, int *buf_index)
{
	(*input)++;
	while (**input && **input != '"')
	{
		if (**input == '$' && ft_wholeisalpha((*input)[1]))
			handle_dollar_sign(input, buffer, buf_index);
		else
			buffer[(*buf_index)++] = *(*input)++;
	}
	(*input)++;
	buffer[*buf_index] = '\0';
	return (1);
}

int	handle_single_quotes(const char **input, char *buffer, int *buf_index)
{
	(*input)++;
	while (**input && **input != '\'')
		buffer[(*buf_index)++] = *(*input)++;
	(*input)++;
	return (0);
}





















































































