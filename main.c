#include "minishell.h"

int	main(int argc, char **argv, char **envp)
{
	char		*input;
	t_command	*command_list;
	t_env	*env_list;

	env_list = malloc(sizeof(t_env));

	(void)argc;
	(void)argv;
	command_list = NULL;
	create_env_list(envp, &env_list);
	while (1)
	{
		signal_handle();
		input = readline("Minishell > ");
		if (input && *input)
			add_history(input);
		if (parse_command_line(input, &command_list))
		{
			printf("Error parsing command line.\n");
			free(input);
			return (1);
		}
		if (ft_isprint(*input))
			test_pipe(command_list, &env_list);
		//print_commands(command_list);
		free(input);
		free_commands(command_list);
		command_list = NULL;
	}
	return (0);
}
