#include "minishell.h"

void	redirect_input(t_command *command)
{
	char	*filename;
	int		fd;

	filename = command->input_file;
	fd = open(filename, O_RDONLY);
	if (fd < 0)
	{
		perror("open");
		exit(EXIT_FAILURE);
	}
	dup2(fd, STDIN_FILENO);
	close(fd);
}

void	redirect_output(t_command *command)
{
	char	*filename;
	int		fd;

	filename = command->output_file;
	fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd < 0)
	{
		perror("open");
		exit(EXIT_FAILURE);
	}
	dup2(fd, STDOUT_FILENO);
	close(fd);
}

void	redirect_append_output(t_command *command)
{
	char	*filename;
	int		fd;

	filename = command->append_file;
	fd = open(filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (fd < 0)
	{
		perror("open");
		exit(EXIT_FAILURE);
	}
	dup2(fd, STDOUT_FILENO);
	close(fd);
}

void redirect_append_input(t_command *command)
{
	char	*filename;
	int		fd;

	filename = command->append_file;
	fd = open(filename, O_RDONLY);
	if (fd < 0)
	{
		perror("open");
		exit(EXIT_FAILURE);
	}
	dup2(fd, STDIN_FILENO);
	close(fd);
}

void	redirect_management(t_command *command)
{
	if (command->input_file)
		redirect_input(command);
	if (command->output_file)
		redirect_output(command);
	if (command->append_file)
		redirect_append_output(command);
}