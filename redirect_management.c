#include "minishell.h"

void	put_into_args(t_command *commands)
{
	int i;

	i = 0;
	while (commands->args[i])
	{
		if (!commands->args[i])
			commands->args[i] = ft_strdup(commands->args[i]);
		i++;
	}
}

int	redirect_input(t_command *commands, t_env **env_list)
{
	char	*filename;
	//int		save_stdin = 0;
	int		fd;

	(void)env_list;
	filename = commands->input_file;
	fd = open(filename, O_RDONLY);
	if (fd < 0)
	{
		ft_printf("bash: %s: No such file or directory\n", commands->input_file);
		commands->exit_code = 1;
		dup2(fd, STDIN_FILENO);
		close(fd);
		return 1;
	}
	//save_stdin = dup(STDIN_FILENO);
	dup2(fd, STDIN_FILENO);
	close (fd);
	put_into_args(commands);
	choose_command(commands, env_list);
	//dup2(save_stdin, STDIN_FILENO);
	//close(save_stdin);
	//dup2(fd, STDIN_FILENO);
	return 0;
}

int	redirect_output(t_command *command, t_env **env_list)
{
	char	*filename;
	int fd;
	int		save_stdout;

	filename = command->output_file;
	fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd < 0)
	{
		ft_printf("%s: No such file or directory\n", command->output_file);
		command->exit_code = 1;
		dup2(fd, STDOUT_FILENO);
		close(fd);
		return 1;
	}
	save_stdout = dup(STDOUT_FILENO);
	dup2(fd, STDOUT_FILENO);
	close (fd);
	choose_command(command, env_list);
	dup2(save_stdout, STDOUT_FILENO);
	close(save_stdout);
	return 0;
}

int	redirect_management(t_command *command, t_env **env_list)
{
	int status;

	status = 0;
	if (command->append_outfile)
		append_file(command);
	if (command->input_file)
		status = redirect_input(command, env_list);
	if (command->output_file)
		redirect_output(command, env_list);
	return status;
}

void	process_input(t_command **command_list, t_env **env_list, char *input, int *save_exit_code)
{
	if (!input)
		return ;
	if (parse_command_line(input, command_list, *save_exit_code))
		*save_exit_code = 2;
	else
	{
		if (ft_isprint(*input))
			select_type(*command_list, env_list);
	}
	*save_exit_code = last_exitcode(*command_list);
	free_command_list(*command_list);
	*command_list = NULL;
}