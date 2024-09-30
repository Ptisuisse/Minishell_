#include "minishell.h"

int	main(int argc, char **argv, char **envp)
{
	char		*input;
	t_command	*command_list;
	t_data		data;

	(void)argc;
	(void)argv;
	command_list = NULL;
	data.env = envp;
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
		// print_commands(command_list);
		free(input);
		execute_commands(command_list);
		free_commands(command_list);
		command_list = NULL;
	}
	return (0);
}
