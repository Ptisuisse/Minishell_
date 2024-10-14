#include "minishell.h"

char	ft_exit_code(t_command *command)
 {
	printf("%d\n", command->exit_code);
 }