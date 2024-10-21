#include "minishell.h"

void	redirect_input(t_command *command)
{
	char	*filename;
	int		fd;

	filename = command->input_file;
	fd = open(filename, O_RDONLY);
	if (fd < 0)
	{
		ft_printf("bash: %s: No such file or directory\n", command->input_file);
		dup2(fd, STDIN_FILENO);
		close(fd);
		return ;
	}
	command->args[1] = command->input_file;
	dup2(fd, STDIN_FILENO);
	close(fd);
	return ;
}

void	redirect_output(t_command *command)
{
	char	*filename;
	int		fd;

	filename = command->output_file;
	fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd < 0)
	{
		ft_printf("%s: No such file or directory", command->output_file);
		dup2(fd, STDOUT_FILENO);
		close(fd);
		return ;
	}
	dup2(fd, STDOUT_FILENO);
	close(fd);
	return ;
}

void	redirect_management(t_command *command)
{

	if (command->append_outfile)
		append_file(command);
	if (command->input_file)
		redirect_input(command);
	if (command->output_file)
		redirect_output(command);
	return ;
}