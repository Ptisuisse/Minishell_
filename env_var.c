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















































































