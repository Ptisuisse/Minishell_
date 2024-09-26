#include "minishell.h"

// int	select_commands(t_command *commands, t_data data)
//{
//	int		result;
//	int		status;
//	pid_t	pid;

//	(void)data;
//	result = 1;
//	while (commands)
//	{
//		// Pipe la prochaine commande si nécessaire
//		if (commands->next)
//			pipe(commands->pipe);
//		pid = fork();
//		if (pid == 0) // Enfant
//		{
//			if (commands->prev) // Pas le premier, donc a un pipe en entrée
//			{
//				dup2(commands->prev->pipe[READ_END], STDIN_FILENO);
//					// Redirige l'entrée
//				close(commands->prev->pipe[READ_END]);
//				close(commands->prev->pipe[WRITE_END]);
//			}
//			if (commands->next) // Pas le dernier, donc a un pipe en sortie
//			{
//				close(commands->pipe[READ_END]);
//				dup2(commands->pipe[WRITE_END], STDOUT_FILENO);
//					// Redirige la sortie
//				close(commands->pipe[WRITE_END]);
//			}
//			choose_command(commands, data); // Exécute la commande
//			exit(0);
//		}
//		else // Parent
//		{
//			// Fermeture des pipes du côté parent
//			if (commands->prev)
//			{
//				close(commands->prev->pipe[READ_END]);
//				close(commands->prev->pipe[WRITE_END]);
//			}
//			commands->pid = pid; // Sauvegarde le PID de l'enfant
//		}
//		commands = commands->next; // Passe à la commande suivante
//	}
//	// Attente de tous les enfants
//	while (wait(&status) > 0);
//	return (result);
//}

 int	select_commands(t_command *commands, t_data data)
{
	int			result;
	t_command	*temp;
	pid_t		pid;

	result = 1;
	(void)data;
	temp = commands;
	while (commands)
	{
		// Crée un pipe pour la prochaine commande, si nécessaire
		if (commands->next)
		{
			if (pipe(commands->pipe) == -1)
			{
				perror("pipe");
				return (-1); // Gérer l'erreur de pipe
			}
		}
		pid = fork(); // Crée un processus fils
		if (pid == 0) // Enfant
		{
			// Si la commande n'est pas la première, redirige l'entrée
			if (commands->prev)
			{
				dup2(commands->prev->pipe[READ_END], STDIN_FILENO);
				// Redirige l'entrée
				close(commands->prev->pipe[READ_END]);
				// Ferme le descripteur de lecture
				close(commands->prev->pipe[WRITE_END]);
				// Ferme le descripteur d'écriture
			}
			// Si la commande n'est pas la dernière, redirige la sortie
			if (commands->next)
			{
				close(commands->pipe[READ_END]);
				// Ferme le descripteur de lecture du pipe
				dup2(commands->pipe[WRITE_END], STDOUT_FILENO);
				// Redirige la sortie
				close(commands->pipe[WRITE_END]);
				// Ferme le descripteur d'écriture
			}
			// Exécute la commande
			choose_command(commands, data);
			exit(0); // Terminaison du processus fils
		}
		else if (pid > 0) // Parent
		{
			// Fermeture des descripteurs de pipe du parent
			if (commands->prev)
			{
				close(commands->prev->pipe[READ_END]);
				// Ferme l'entrée du pipe précédent
				close(commands->prev->pipe[WRITE_END]);
				// Ferme la sortie du pipe précédent
			}
			commands->pid = pid; // Sauvegarde le PID de l'enfant
		}
		else
		{
			perror("fork"); // Gérer l'erreur de fork
			return (-1);
		}
		commands = commands->next; // Passe à la commande suivante
	}
	return (result);
}


// int	select_commands(t_command *commands, t_data data)
//{
//	int	result;
//	int	status;
//	int	i;
//	int	j;

//	(void)data;
//	result = 1;
//	i = 0;
//	j = 0;
//	while (commands)
//	{
//		if (commands->next != NULL && commands->prev == NULL) // le 1er X |
//		{
//			printf("1ere commande suivie par un | \n");
//			// COMMAND |
//			commands->pid = fork();
//			if (commands->pid == 0)
//			{
//				close(commands->pipe[READ_END]);
//				dup2(commands->pipe[WRITE_END], STDOUT_FILENO);
//				close(commands->pipe[WRITE_END]);
//				choose_command(commands, data);
//			}
//			else
//			{
//				close(commands->pipe[WRITE_END]);
//			}
//			result = 0;
//		}
//		else if (commands->next != NULL && commands->prev != NULL)
//			// entre | X |
//		{
//			//| COMMAND |
//			printf("commande entre 2 | \n");
//			commands->pid = fork();
//			if (commands->pid == 0)
//			{
//				close(commands->pipe[READ_END]);
//				dup2(commands->prev->pipe[READ_END], STDIN_FILENO);
//				close(commands->prev->pipe[READ_END]);
//				dup2(commands->pipe[WRITE_END], STDOUT_FILENO);
//				close(commands->pipe[WRITE_END]);
//				choose_command(commands, data);
//			}
//			else
//			{
//				close(commands->prev->pipe[READ_END]);
//				close(commands->pipe[WRITE_END]);
//			}
//			result = 0;
//		}
//		else if (commands->next == NULL && commands->prev != NULL)
//			// le dernier | X
//		{
//			// | COMMAND
//			printf("Derniere commande\n");
//			commands->pid = fork();
//			if (commands->pid == 0)
//			{
//				close(commands->pipe[READ_END]);
//				dup2(commands->prev->pipe[READ_END], STDIN_FILENO);
//				close(commands->prev->pipe[READ_END]);
//				close(commands->pipe[WRITE_END]);
//				choose_command(commands, data);
//			}
//			else
//			{
//				close(commands->prev->pipe[READ_END]);
//			}
//			result = 0;
//		}
//		else if (commands->next == NULL && commands->prev == NULL)
// 1 seule node X
//			result = choose_command(commands, data);
//		waitpid(commands->pid, &status, 0);
//		commands = commands->next;
//		j++;
//	}
//	return (result);
//}

void	ft_pipe(t_command *command, t_data data)
{
	int	status;
	int	pid;

	pipe(command->pipe);
	pid = fork(); // pid = 0
	if (pid == 0) // enfant
	{
		close(command->pipe[0]);
		dup2(command->pipe[1], STDOUT_FILENO);
		close(command->pipe[1]);
		choose_command(command, data);
	}
	else // parent
	{
		close(command->pipe[0]);
		close(command->pipe[1]);
	}
	waitpid(command->pid, &status, WNOHANG);
}
