#include "minishell.h"

void    select_type(t_command *command, t_env **list)
{
	int save_in = dup(STDIN_FILENO);
	int save_out = dup(STDOUT_FILENO);

	check_heredoc(command);
	if (command->next)
		commands_manager(command, list);
	else
	{
		if (command->input_file || command->output_file || command->append_file || command->append_file)
			redirect_management(command, list);
		else
    		choose_command(command, list);
	}

	dup2(save_out, STDOUT_FILENO);
	dup2(save_in, STDIN_FILENO);
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

//void	ft_close_fd(t_command *commands)
//{
//    t_command	*cmd;

//    cmd = commands;
//    while (commands)
//    {
//        if (commands->pipe[0] >= 0)
//            close(commands->pipe[0]);
//        if (commands->pipe[1] >= 0)
//            close(commands->pipe[1]);
//        if (commands->input_fd >= 0)
//            close(commands->input_fd);
//        if (commands->output_fd >= 0)
//            close(commands->output_fd);
//        if (commands->append_infd >= 0)
//            close(commands->append_infd);
//        if (commands->append_outfd >= 0)
//            close(commands->append_outfd);
//        commands = commands->next;
//    }
//    commands = cmd;
//}

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
			ft_process_wait(commands);
			if (commands->status == 256)
				commands->exit_code = 1;
			commands = commands->next;
		}
	}
	//ft_close_fd(commands);
	commands = cmd;
	// if (count > 0)
	// 	check_error_file(commands);
}