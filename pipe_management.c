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

// void	ft_pipe(t_command *command, t_data data)
//{
//	int	status;
//	int	pid;

//	pipe(command->pipe);
//	pid = fork();
//	if (pid == 0)
//	{
//		close(command->pipe[0]);
//		dup2(command->pipe[1], STDOUT_FILENO);
//		close(command->pipe[1]);
//		choose_command(command, data);
//	}
//	else
//		close(command->pipe[0]);
//	waitpid(pid, &status, 0);
//}

void	ft_pipe(t_command *command, t_data data)
{
	int	pid;
	int	status;

	int pipefd[2]; // Descripteurs de pipe
	if (pipe(pipefd) == -1)
	{
		perror("pipe");
		exit(EXIT_FAILURE);
	}
	pid = fork(); // Fork pour exécuter la commande
	if (pid == -1)
	{
		perror("fork");
		exit(EXIT_FAILURE);
	}
	if (pid == 0) // Enfant
	{
		// Si la commande précédente existe, redirige l'entrée vers le pipe
		if (command->prev != NULL)
		{
			dup2(command->prev->pipe[0], STDIN_FILENO);
				// Redirige entrée standard vers le pipe précédent
			close(command->prev->pipe[0]);
				// Ferme la lecture du pipe précédent
			close(command->prev->pipe[1]);
				// Ferme l'écriture du pipe précédent
		}
		// Si ce n'est pas la dernière commande, redirige la sortie vers le pipe
		if (command->next != NULL)
		{
			dup2(pipefd[1], STDOUT_FILENO);
				// Redirige la sortie standard vers l'écriture du pipe
			close(pipefd[0]);               // Ferme l'extrémité de lecture
			close(pipefd[1]);               // Ferme l'extrémité d'écriture
		}
		// Exécute la commande ou le builtin
		choose_command(command, data);
		exit(EXIT_SUCCESS); // On termine le processus enfant
	}
	else // Parent
	{
		// Si une commande précédente existe, ferme les pipes de cette commande
		if (command->prev != NULL)
		{
			close(command->prev->pipe[0]);
				// Ferme l'extrémité de lecture du pipe précédent
			close(command->prev->pipe[1]);
				// Ferme l'extrémité d'écriture du pipe précédent
		}
		if (command->next != NULL)
		{
			command->pipe[0] = pipefd[0];
				// L'extrémité de lecture du pipe actuel devient entrée pour la prochaine commande
			command->pipe[1] = pipefd[1]; // L'extrémité d'écriture
		}
		// Attendre la fin du processus enfant
		waitpid(pid, &status, 0);
	}
}
