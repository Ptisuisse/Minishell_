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

int	redirect_input(t_command *commands)
{
	char	*filename;
	int		fd;

	if (commands->input_fd)
		filename = commands->input_file;
	else
		filename = commands->append_infile;
	fd = open(filename, O_RDONLY);
	if (fd < 0)
	{
		//ft_printf("bash: %s: No such file or directory\n", commands->input_file);
		commands->exit_code = 1;
		close(fd);
		return (0);
	}
	if (access(filename, W_OK) == -1)
    {
        //ft_printf("%s: Permission denied\n", filename);
        commands->exit_code = 1;
        return 0;
    }
	close (fd);
	return (1);
}

void	redirect_input2(t_command *commands, t_env **env_list)
{
	char	*filename;
	int		save_stdin;
	int		fd;

	filename = commands->input_file;
	fd = open(filename, O_RDONLY);
	if (fd < 0)
	{
		ft_printf("bash: %s: No such file or directory\n", commands->input_file);
		commands->exit_code = 1;
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

int	redirect_output(t_command *commands)
{
	char	*filename;
	int fd;

	if (commands->output_fd)
		filename = commands->output_file;
	else
		filename = commands->append_outfile;
	if (access(filename, F_OK) == -1)
    {
        ft_printf("%s: No such file or directory\n", filename);
        commands->exit_code = 1;
        return 0;
    }
	if (access(filename, W_OK) == -1)
    {
        ft_printf("%s: Permission denied\n", filename);
        commands->exit_code = 1;
        return 0;
    }
	fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd < 0)
	{
		ft_printf("%s: No such file or directory\n", commands->output_file);
		commands->exit_code = 1;
		close(fd);
		return 0;
	}
	close (fd);
	return 1;
}

void	redirect_output2(t_command *commands)
{
	char	*filename;
	int fd;
	int		save_stdout;

	filename = commands->output_file;
	fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd < 0)
	{
		ft_printf("%s: No such file or directory\n", commands->output_file);
		commands->exit_code = 1;
		dup2(fd, STDOUT_FILENO);
		close(fd);
		return ;
	}
	save_stdout = dup(STDOUT_FILENO);
	dup2(fd, STDOUT_FILENO);
	close (fd);
	choose_command(commands, NULL);
	dup2(save_stdout, STDOUT_FILENO);
	close(save_stdout);
	return ;
}

void	redirect_management(t_command *command, t_env **env_list)
{

	if (command->append_outfile)
		append_file(command, env_list);
	if (command->input_file)
		redirect_input2(command, env_list);
	if (command->output_file)
		redirect_output2(command);
	return ;
}
void	process_input(t_command **command_list, t_env **env_list, char *input, int *save_exit_code)
{
	if (parse_command_line(input, command_list, *save_exit_code))
		(*command_list)->exit_code = 2;
	else
	{
		check_heredoc(*command_list);
		//	grave probleme ici
		if (ft_isprint(*input))
			commands_manager(*command_list, env_list);
	}
	*save_exit_code = last_exitcode(*command_list);
	free_command_list(*command_list);
	*command_list = NULL;
}