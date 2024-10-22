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

void	redirect_input(t_command *commands, t_env **env_list)
{
	char	*filename;
	int		save_stdin;
	int		fd;

	filename = commands->input_file;
	fd = open(filename, O_RDONLY);
	if (fd < 0)
	{
		ft_printf("bash: %s: No such file or directory\n", commands->input_file);
		dup2(fd, STDIN_FILENO);
		close(fd);
		return ;
	}
	save_stdin = dup(STDIN_FILENO);
	dup2(fd, STDIN_FILENO);
	close (fd);
	put_into_args(commands);
	choose_command(commands, env_list);
	dup2(save_stdin, STDIN_FILENO);
	close(save_stdin);
	//dup2(fd, STDIN_FILENO);
	return ;
}

void	redirect_output(t_command *commands, t_env **env_list)
{
	char	*filename;
	int fd;
	int		save_stdout;

	filename = commands->output_file;
	fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd < 0)
	{
		ft_printf("%s: No such file or directory", commands->output_file);
		dup2(fd, STDOUT_FILENO);
		close(fd);
		return ;
	}
	save_stdout = dup(STDOUT_FILENO);
	dup2(fd, STDOUT_FILENO);
	close (fd);
	choose_command(commands, env_list);
	dup2(save_stdout, STDOUT_FILENO);
	close(save_stdout);
	return ;
}

void	redirect_management(t_command *command, t_env **env_list)
{

	if (command->append_outfile)
		append_file(command, env_list);
	if (command->input_file)
		redirect_input(command, env_list);
	if (command->output_file)
		redirect_output(command, env_list);
	return ;
}