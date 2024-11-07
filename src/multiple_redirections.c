#include "minishell.h"

void	multiple_append_child(t_command *command)
{
	int	pipe_fd;

	pipe_fd = open(command->append_file, O_CREAT | O_WRONLY | O_APPEND, 0644);
	if (pipe_fd < 0)
	{
			command->exit_code = 1;
			return ;
	}
	dup2(pipe_fd, STDOUT_FILENO);
	close(pipe_fd);
	return ;
}

void	multiple_append_file(t_command *command)
{
	multiple_append_child(command);
	command->append_file = NULL;
	return ;
}

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
	command->output_file = NULL;
	return (0);
}

void	multiple_redirection(t_command *command, t_env **env_list)
{
	while (command->file)
	{
		if (command->input_file != NULL)
			multiple_redirection_input(command, env_list);
		if (command->output_file != NULL && command->output == 4)
			multiple_redirection_output(command, env_list);
		if (command->append_file != NULL && command->output == 3)
			multiple_append_file(command);
		command->file--;
	}
	exec_command(command, env_list);
}