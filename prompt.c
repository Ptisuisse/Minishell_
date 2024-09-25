#include "minishell.h"

int	main(int argc, char **argv, char **envp)
{
	(void)argc;
	(void)argv;

	char *input;
	int status;
	int exec;
	t_command *temp;
	t_command *command_list = NULL;
	t_data data;
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
		temp = command_list;
		// command_manager(command_list, data);
		while (command_list)
		{
			exec = select_commands(command_list, data);
			if (exec == -1)
				exit(1);
			command_list = command_list->next;
		}
		temp = ft_lstlst(temp);
		while (temp)
		{
			waitpid(temp->pid, &status, 0);
			temp = temp->prev;
		}
		free_commands(command_list);
		free_commands(temp);
		////		add_history(input);
	}
	return (0);
}