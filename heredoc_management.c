#include "minishell.h"

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
