#include "minishell.h"

int	main(int argc, char **argv, char **envp)
{
	char		*input;
	t_command	*command_list;
	t_data		data;
	int			wpid;
	int			status2;

	// int			exec;
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
		select_commands(command_list, data);
		wpid = 0;
		int status = 0;
		status2 = 0;
		while (wpid != -1)
		{
			wpid = waitpid(-1, &status, 0);
			if (wpid == command_list->pid)
				status2 = status;
		}
		free_commands(command_list);
		command_list = NULL;
		////		add_history(input);
	}
	return (0);
}
