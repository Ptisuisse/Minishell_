#include "minishell.h"

int	main(int argc, char **argv, char **envp)
{
	(void)argc;
	(void)argv;

	char *input;
	int exec;
	t_command *command_list = NULL;
	t_data data;
	int in = dup(STDIN_FILENO);
	int out = dup(STDOUT_FILENO);
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
		//print_commands(command_list);
		while (command_list)
		{
			 exec = select_commands(command_list, data);
			 if (exec == -1)
				exit(1);
			command_list = command_list->next;
		}
		free_commands(command_list);
		dup2(in, STDIN_FILENO);
		dup2(out, STDOUT_FILENO);
		//free_commands(temp);
		////		add_history(input);
		//		free(input);
	}
	return (0);
}