#include "minishell.h"

void	test_pipe(t_command *commands, t_env **env_list)
{
	//else if (commands->append_outfd == 1)
	//	wait_output(commands, env_list);
	//else if (commands->input_fd == 1)
	//	redirect_input(commands);
	//else if (commands->output_fd == 1)
	//	redirect_output(commands);
	// if (commands->next != NULL)
	commands_manager(commands, env_list);
	// else
		// choose_command(commands, env_list);
}

void	wait_output(t_command *command, t_env **env_list)
{
	char	*input;
	char	*end_of_input;
	int	fd[2];

	input = NULL;
	if (command->append_outfd == 1)
		end_of_input = command->append_outfile;
	else
	{
		perror("open");
		exit(EXIT_FAILURE);
	}
	while (1)
	{
		input = readline("> ");
		if (strcmp(input, end_of_input) == 0)
			break;
		write(fd[1], input, strlen(input));
		write(fd[1], "\n", 1);
		free(input);
	}
	close(fd[WRITE_END]);
	dup2(fd[READ_END], STDIN_FILENO);
	close(fd[READ_END]);
	//free(input);
	choose_command(command, env_list);
}

void	wait_input(t_command *command, t_env **env_list)
{
	char	*input;
	char	*end_of_input;
	int		heredoc_fd;
	int stdin_backup;

	(void)env_list;
	input = NULL;
	end_of_input = NULL;
	stdin_backup = dup(STDIN_FILENO);
	if (command->append_infd == 1)
		end_of_input = command->append_infile;
	heredoc_fd = open(".heredoc", O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (heredoc_fd == -1)
	{
		perror("Failed to open .heredoc");
	}
	while (1)
	{
		input = readline("> ");
		if (!input)
		{
			perror("readline error");
			close(heredoc_fd);
			exit(EXIT_FAILURE);
		}
		if (strcmp(input, end_of_input) == 0)
		{
			free(input);
			break;
		}
		if (write(heredoc_fd, input, strlen(input)) == -1)
		{
			perror("write error");
			free(input);
			close(heredoc_fd);
		}
		if (write(heredoc_fd, "\n", 1) == -1)
		{
			perror("write error");
			free(input);
			close(heredoc_fd);
		}
		free(input);
	}
	close(heredoc_fd);
	heredoc_fd = open(".heredoc", O_RDONLY);
	if (heredoc_fd == -1)
		perror("Failed to reopen .heredoc");
	if (dup2(heredoc_fd, STDIN_FILENO) == -1)
	{
		perror("dup2 error");
		close(heredoc_fd);
	}
	close(heredoc_fd);
	dup2(stdin_backup, STDIN_FILENO);
	close(stdin_backup);
	command->args[1] = ".heredoc";
	// choose_command(command, env_list);
	return ;
}


//void	wait_input(t_command *command, t_env **env_list)
//{
//	char	*input;
//	char	*end_of_input;
//	int		fd;

//	input = NULL;
//	if (command->append_infd == 1)
//		end_of_input = command->append_infile;
//	fd = open(".heredoc", O_CREAT | O_WRONLY | O_TRUNC, 0644);

//	while (1)
//	{
//		input = readline("> ");
//		if (strcmp(input, end_of_input) == 0)
//			break;
//		else
//		{
//			write(fd, input, strlen(input));
//			write(fd, "\n", 1);
//		}
//		free(input);
//	}
//	close(fd);
//	fd = open(".heredoc", O_RDONLY);
//	dup2(fd, STDIN_FILENO);
//	close(fd);
//	command->args[1] = ".heredoc";
//	//free(input);
//	choose_command(command, env_list);
//}

void	ft_process_wait(t_command *commands)
{
	int status = 0;
	t_command *cmd;

	cmd = commands;
	while (commands)
	{
		waitpid(commands->pid, &status, 0);
		commands = commands->next;
	}
	commands = cmd;
}

void	commands_manager(t_command *commands, t_env **env_list)
{
	t_command	*cmd;
	//int			status;

	cmd = commands;
	if (commands->next == NULL)
	{
		choose_command(commands, env_list);
		return ;
	}
	while (commands)
	{
		if (commands->append_infd == 1)
			wait_input(commands, env_list);
		else if (commands->append_outfd == 1)
			wait_output(commands, env_list);
		else if (commands->input_fd == 1)
			redirect_input(commands);
		else if (commands->output_fd == 1)
			redirect_output(commands);
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

int	check_path(char *pathname)
{
	if (!ft_strncmp(pathname, "/bin/", 5))
		return (1);
	return (0);
}

int	exec_command(char *pathname, char **args)
{
	int pid;
	char *path;
	int status;

	status = 0;
	if (!check_path(pathname))
		path = ft_strjoin("/bin/", pathname);
	else
		path = ft_strdup(pathname);
	pid = fork();
	if (pid == 0)
	{
		if (execve(path, args, NULL) == -1)
		{
			g_exit_code = 127;
			ft_printf("%s: command not found\n", pathname);
			return (0);
		}
	}
	free(path);
	waitpid(pid, NULL, 0);
	return (1);
}
