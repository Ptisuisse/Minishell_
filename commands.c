#include "minishell.h"

void    select_type(t_command *command, t_env **list)
{
	int save_in = dup(STDIN_FILENO);
	int save_out = dup(STDOUT_FILENO);

	//check_heredoc(command);
	if (command->next)
		commands_manager(command, list);
	else
	{
		if (command->file > 0)
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
		}
		cmd = cmd->next;
	}
	cmd = head;
}

void commands_manager(t_command *commands, t_env **env_list)
{
   t_command *cmd = commands;
   int prev_pipe_fd = -1;
	int status;
   int last_exit_code;

   while (commands)
   {
       if (commands->error_file > 0)
           commands = commands->next;
       if (commands->next && pipe(commands->pipe) == -1)
       {
           perror("pipe error");
           exit(EXIT_FAILURE);
       }
       commands->pid = fork();
       if (commands->pid == 0)
       {
           if (prev_pipe_fd != -1)
           {
               dup2(prev_pipe_fd, STDIN_FILENO);
               close(prev_pipe_fd);
           }
           if (commands->next)
           {
               close(commands->pipe[0]);
               dup2(commands->pipe[1], STDOUT_FILENO);
               close(commands->pipe[1]);
           }
			if (commands->file > 0)
           		redirect_management(commands, env_list);
           else
               choose_command_pipe(commands, env_list);
           exit(EXIT_SUCCESS);
       }
       else if (commands->pid > 0)
       {
           if (prev_pipe_fd != -1)
               close(prev_pipe_fd);

           if (commands->next)
               close(commands->pipe[1]);

           prev_pipe_fd = commands->pipe[0];
       }
       else
       {
           perror("fork error");
           exit(EXIT_FAILURE);
       }
       commands = commands->next;
   }
   commands = cmd;
   while (commands)
   {
       waitpid(commands->pid, &status, 0);
        if (WIFEXITED(status))
            last_exit_code = WEXITSTATUS(status);
        commands->exit_code = last_exit_code;
       commands = commands->next;
   }
   commands = cmd;
   check_error_file(commands);
}

//void	commands_manager(t_command *commands, t_env **env_list)
//{
//	t_command	*cmd;
//	int count = 0;

//	cmd = commands;
//	while (commands)
//	{
//		if (commands->error_file == 1)
//		{
//			while (commands->error_file == 1)
//			{
//				count += 1;
//				commands = commands->next;
//			}
//		}
//		setup_pipes(commands);
//		if (commands->pid == 0)
//		{
//			handle_child_process(commands);
//			if (commands->file > 0)
//				redirect_management(commands, env_list);
//			else
//				choose_command(commands, env_list);
//			exit(0);
//		}
//		else
//		{
//			ft_process_wait(commands);
//			if (commands->status == 256)
//				commands->exit_code = 1;
//			commands = commands->next;
//		}
//	}
//	ft_process_wait(commands);
//	ft_close_fd(commands);
//	commands = cmd;
//	if (count > 0)
//	 	check_error_file(commands);
//}

void ft_close_fd(t_command *commands)
{
    while (commands)
    {
        close(commands->pipe[0]);
        close(commands->pipe[1]);
        commands = commands->next;
    }
}
