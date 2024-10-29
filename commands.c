#include "minishell.h"

void    select_type(t_command *command, t_env **list)
{
    check_heredoc(command);
    if (command->next)
        commands_manager(command, list);
    else
	{
		if (command->input_file || command->output_file || command->append_infile || command->append_outfile)
			redirect_management(command, list);
		else
    		choose_command(command, list);
	}
}

void	check_error_file(t_command *cmd)
{
	t_command	*head;

	head = cmd;
	while (cmd)
	{
		if (cmd->error_file == 1)
		{
			redirect_input(cmd, NULL);
			cmd->exit_code = 1;
			break ;
		}
		cmd = cmd->next;
	}
	cmd = head;
}

void	commands_manager(t_command *commands, t_env **env_list)
{
	t_command	*cmd;
	int count = 0;

	cmd = commands;
	while (commands)
	{
		if (commands->error_file == 1)
		{
			count += 1;
			commands = commands->next;
		}
		setup_pipes(commands);
		if (commands->pid == 0)
		{
			handle_child_process(commands);
			choose_command(commands, env_list);
			exit(0);
		}
		else
		{
			handle_parent_process(commands);
			if (commands->next == NULL)
				ft_process_wait(commands);
			if (commands->status == 256)
				commands->exit_code = 1;
			commands = commands->next;
		}
	}
	ft_process_wait(commands);
	commands = cmd;
	if (count > 0)
		check_error_file(commands);
}