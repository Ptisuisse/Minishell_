#include "minishell.h"

int	select_commands(t_command *commands, t_data data)
{
	int	result;

	if (commands->next != NULL)
	{
		ft_pipe(commands, data);
		result = 0;
	}
	else
		result = choose_command(commands, data);
	return (result);
}

void	ft_pipe(t_command *command, t_data data)
{
	(void)data;
	pipe(command->pipe);
	command->pid = fork();
	if (command->pid == 0)
	{
		if (command->prev != NULL)
		{
			dup2(command->prev->pipe[0], STDIN_FILENO);
			close(command->prev->pipe[0]);
		}
		if (command->next != NULL)
		{
			close(command->pipe[0]);
			dup2(command->pipe[1], STDOUT_FILENO);
			close(command->pipe[1]);
		}
		choose_command(command, data);
		// Exécuter la commande ici (par exemple, execvp)
	}
	else
	{
		if (command->prev != NULL)
			close(command->prev->pipe[0]);
		if (command->next != NULL)
			close(command->pipe[1]);
		waitpid(command->pid, NULL, 0);
	}
}


// close(command->pipe[0]);
// dup2(command->pipe[1], STDOUT_FILENO);
// close(command->pipe[1]);
// choose_command(command, data);
// void	ft_pipe(t_command *command, t_data data)
//{
//	int pid;
//	int status;

//	int pipefd[2];
//	if (pipe(pipefd) == -1)
//	{
//		perror("pipe");
//		exit(EXIT_FAILURE);
//	}
//	pid = fork();
//	if (pid == -1)
//	{
//		perror("fork");
//		exit(EXIT_FAILURE);
//	}
//	if (pid == 0)
//	{
//		if (command->prev != NULL)
//		{
//			dup2(command->prev->pipe[0], STDIN_FILENO);
//			close(command->prev->pipe[0]);
//			close(command->prev->pipe[1]);
//		}
//		if (command->next != NULL)
//		{
//			dup2(pipefd[1], STDOUT_FILENO);
//			close(pipefd[0]);
//			close(pipefd[1]);
//		}
//		choose_command(command, data);
//		exit(EXIT_SUCCESS);
//	}
//	else
//	{
//		if (command->prev != NULL)
//		{
//			close(command->prev->pipe[0]);
//			close(command->prev->pipe[1]);
//		}
//		if (command->next != NULL)
//		{
//			command->pipe[0] = pipefd[0];
//			command->pipe[1] = pipefd[1];
//		}
//		waitpid(pid, &status, 0);
//	}
//}

/*ANCIENNE VERSION*/

// int	select_commands(t_command *commands, t_data data)
//{
//	int	result;
//	int	pipe_std[2];

//	result = -1;
//	(void)data;
//	pipe(pipe_std);
//	if (commands->next != NULL)
//	{
//		ft_pipe(commands, data);
//		result = 0;
//	}
//	else
//	{
//		result = choose_command(commands, data);
//	}
//	return (result);
//}

/*ANCIENNE VERSION*/