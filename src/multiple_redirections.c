#include "minishell.h"

void	multiple_redirection_exec(t_command *command, t_env **env_list)
{
	char	*cmd;
	char	**envp;

	envp = create_envp(*env_list);
	cmd = ft_strdup(command->args[0]);
	if (!check_path(command->args[0]))
		cmd = find_path(env_list, command->args[0]);
	command->pid = fork();
	if (command->pid == 0)
	{
		if (execve(cmd, command->args, envp) == -1)
		{
			command->exit_code = 127;
			ft_printf_error("%s: command not found\n", command->args[0]);
			free(cmd);
			free(envp);
			exit(command->exit_code);
		}
	}
	else
		ft_process_wait(command);
	command->exit_code = WEXITSTATUS(command->status);
	free(cmd);
	free_split(envp);
	return ;
}

int	multiple_redirection_input(t_command *command, t_env **env_list)
{
	char	*filename;
	int		fd;

	(void)env_list;
	filename = command->input_file;
	fd = open(filename, O_RDONLY);
	if (fd < 0)
	{
		command->exit_code = 1;
		return (1);
	}
	dup2(fd, STDIN_FILENO);
	close(fd);
	put_into_args(command);
	command->input_file = NULL;
	return (0);
}

int	multiple_redirection_output(t_command *command, t_env **env_list)
{
	char	*filename;
	int		fd;

	(void)env_list;
	filename = command->output_file;
	fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd < 0)
	{
		command->exit_code = 1;
		return (1);
	}
	dup2(fd, STDOUT_FILENO);
	close(fd);
	put_into_args(command);
	command->output_file = NULL;
	return (0);
}

void	multiple_redirection(t_command *command, t_env **env_list)
{
	while (command->file)
	{
		if (command->input_file != NULL)
			multiple_redirection_input(command, env_list);
		if (command->append_file != NULL)
			append_file(command);
		if (command->output_file != NULL)
			multiple_redirection_output(command, env_list);
		command->file--;
	}
	multiple_redirection_exec(command, env_list);
}