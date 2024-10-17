#include "minishell.h"

void echo_cmd(char **args)
{
	int newline;
	int i;
	int	j;

	newline = 1;
	i = 1;
	while (args[i] && args[i][0] == '-')
	{
		j = 1;
		while (args[i][j] == 'n')
			j++;
		if (args[i][j] == '\0')
			newline = 0;
		else
			break;
		i++;
	}
	while (args[i])
	{
		printf("%s", args[i++]);
		if (args[i])
			printf(" ");
	}
	if (newline)
		printf("\n");
	g_exit_code = 0;
}
