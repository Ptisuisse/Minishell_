#include "minishell.h"

void    select_type(t_command *command, t_env **list)
{
    check_heredoc(command);
    if (command->next)
        commands_manager(command, list);
    else
        choose_command(command, list);
}

void	commands_manager(t_command *commands, t_env **env_list)
{
	t_command	*cmd;
	int count = 0;

	cmd = commands;
	while (commands)
	{
		setup_pipes(commands);
		if (commands->pid == 0)
		{
			handle_child_process(commands);
			if (commands->input_fd || commands->output_fd || commands->append_infd || commands->append_outfd)
			{
            	if (!redirect_management(commands, env_list))
                    count++;
            }
            else
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
}