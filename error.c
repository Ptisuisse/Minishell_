#include "minishell.h"

char	*replace_by_exit_code(char *result, int *result_index)
{
	char	*exit_code;
	int		i;

	i = 0;
	exit_code = ft_itoa(g_exit_code);
	while (exit_code[i])
		result[(*result_index)++] = exit_code[i++];
	free(exit_code);
	return (result);
}