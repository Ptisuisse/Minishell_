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
	create_env_list(&data);
	//printf_list(data.env_list);
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
		//print_commands(command_list);
		free(input);
		commands_manager(command_list, &data);
		free_commands(command_list);
		command_list = NULL;
	}
	return (0);
}
