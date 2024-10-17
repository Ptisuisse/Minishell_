#include "minishell.h"

void	commands_manager(t_command *commands, t_env **env_list)
{
	t_command	*cmd;

	cmd = commands;
	if (commands->next == NULL)
	{
		choose_command(commands, env_list);
		return ;
	}
	while (commands)
	{
		if (commands->next)
			pipe(commands->pipe);
		commands->pid = fork();
		if (commands->pid == 0)
		{
			if (commands->prev)
			{
				dup2(commands->prev->pipe[READ_END], STDIN_FILENO);
				close(commands->prev->pipe[READ_END]);
			}
			if (commands->next)
			{
				close(commands->pipe[READ_END]);
				dup2(commands->pipe[WRITE_END], STDOUT_FILENO);
				close(commands->pipe[WRITE_END]);
			}
			choose_command(commands, env_list);
			exit(EXIT_SUCCESS);
		}
		else
		{
			if (commands->next)
				close(commands->pipe[WRITE_END]);
			if (commands->prev)
				close(commands->prev->pipe[READ_END]);
			commands = commands->next;
		}
	}
	commands = cmd;
	ft_process_wait(commands);
}

int	choose_command(t_command *command, t_env **env_list)
{
	int	result;

	result = -1;
	if (command->append_infd == 1)
		wait_input(command, env_list);
	if (check_builtins(command, env_list))
		result = 0;
	else
	{
		exec_command(command->args[0], command->args);
		result = 0;
	}
	return (result);
}

void	start_builtins(t_command *command, t_env **env_list)
{
	if (!(ft_strcmp(command->args[0], "exit")))
		exit_cmd(command);
	else if (!(ft_strcmp(command->args[0], "echo")))
		echo_cmd(command->args);
	else if (!(ft_strcmp(command->args[0], "cd")))
		cd_cmd(command, *env_list);
	else if (!(ft_strcmp(command->args[0], "pwd")))
		pwd_cmd();
	else if (!(ft_strcmp(command->args[0], "env")))
		env_cmd(*env_list);
	else if (!(ft_strcmp(command->args[0], "export")))
		*env_list = export_cmd(*env_list, command);
	else if (!(ft_strcmp(command->args[0], "unset")))
		unset_cmd(command, *env_list);
	else if (!(ft_strcmp(command->args[0], "clear")))
		clear_cmd();
}
