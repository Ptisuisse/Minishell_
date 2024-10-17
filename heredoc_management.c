/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_management.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lvan-slu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/17 13:05:22 by lvan-slu          #+#    #+#             */
/*   Updated: 2024/10/17 13:05:22 by lvan-slu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void go_heredoc(t_command *command, int fd_doc)
{
    char *input;
    char *end_of_input;

    input = NULL;
    end_of_input = command->append_infile; // Le délimiteur du heredoc (ex: "EOF")
    while (1)
    {
        input = readline("> ");
        if (!input)
        {
            perror("readline error");
            exit(EXIT_FAILURE); // Quitter si readline échoue
        }
        if (strcmp(input, end_of_input) == 0) // Vérifie si le délimiteur est atteint
        {
            free(input);
            break; // Sort de la boucle si délimiteur atteint
        }
        ft_putstr_fd(input, fd_doc); // Écrit l'entrée dans le pipe ou fichier heredoc
        ft_putstr_fd("\n", fd_doc);  // Ajoute une nouvelle ligne
        free(input); // Libère la mémoire après utilisation
    }
    close(fd_doc); // Fermer le descripteur de fichier après avoir atteint le délimiteur
}

static void child_heredoc(t_command *command, int *heredoc_pipe)
{
    close(heredoc_pipe[0]); // Fermer la lecture dans l'enfant
    go_heredoc(command, heredoc_pipe[1]); // Appeler go_heredoc pour capturer l'entrée
    // La fermeture de heredoc_pipe[1] est déjà gérée dans go_heredoc
    exit(0); // Quitter proprement l'enfant
}

static int	parent_heredoc(t_command *command, int *heredoc)
{
    int exit_status;
    int std_in;

	(void)command;
    // Attendre la fin du processus enfant
    wait(&exit_status);

    exit_status = WEXITSTATUS(exit_status);
    if (exit_status == 1)
        return (-3);
    else {
        // Rediriger le descripteur de fichier du pipe vers stdin
        std_in = dup2(heredoc[0], STDIN_FILENO);
        if (std_in == -1) {
            perror("dup2");
            return (-1);
        }
        close(heredoc[0]);
        return (-1);
    }
	return (0);
}

int heredoc(t_command *command, t_env **env_list)
{
    int heredoc_pipe[2];
    int pid;

    (void)env_list;
    if (pipe(heredoc_pipe) == -1)
    {
        perror("pipe error");
        return (-1);
    }
    pid = fork(); // Fork pour créer un processus enfant pour le heredoc
    if (pid == 0) // Dans l'enfant
        child_heredoc(command, heredoc_pipe); // Gérer le heredoc dans l'enfant
    else // Dans le parent
        return parent_heredoc(command, heredoc_pipe); // Attendre l'enfant et gérer les pipes
    return (-1);
}

//void	wait_output(t_command *command, t_env **env_list)
//{
//	char	*input;
//	char	*end_of_input;
//	int	fd[2];

//	input = NULL;
//	if (command->append_outfd == 1)
//		end_of_input = command->append_outfile;
//	else
//	{
//		perror("open");
//		exit(EXIT_FAILURE);
//	}
//	while (1)
//	{
//		input = readline("> ");
//		if (strcmp(input, end_of_input) == 0)
//			break;
//		ft_putstr_fd(input, fd_doc);
//		ft_putstr_fd("\n", fd_doc);
//		free(input);
//	}
//	close(fd[WRITE_END]);
//	dup2(fd[READ_END], STDIN_FILENO);
//	close(fd[READ_END]);
//	//free(input);
//	choose_command(command, env_list);
//}

////void	wait_input(t_command *command, t_env **env_list)
////{
////	char	*input;
////	char	*end_of_input;
////	int		heredoc_fd;
////	int		stdin_backup;

////	(void)env_list;
////	input = NULL;
////	end_of_input = NULL;
////	stdin_backup = dup(STDIN_FILENO);
////	if (command->append_infd == 1)
////		end_of_input = command->append_infile;
////	heredoc_fd = open(".heredoc", O_CREAT | O_WRONLY | O_TRUNC, 0644);
////	if (heredoc_fd == -1)
////	{
////		perror("Failed to open .heredoc");
////	}
////	while (1)
////	{
////		input = readline("> ");
////		if (!input)
////		{
////			perror("readline error");
////			close(heredoc_fd);
////			exit(EXIT_FAILURE);
////		}
////		if (strcmp(input, end_of_input) == 0)
////		{
////			free(input);
////			break;
////		}
////		if (write(heredoc_fd, input, strlen(input)) == -1)
////		{
////			perror("write error");
////			free(input);
////			close(heredoc_fd);
////		}
////		if (write(heredoc_fd, "\n", 1) == -1)
////		{
////			perror("write error");
////			free(input);
////			close(heredoc_fd);
////		}
////		free(input);
////	}
////	close(heredoc_fd);
////	heredoc_fd = open(".heredoc", O_RDONLY);
////	if (heredoc_fd == -1)
////		perror("Failed to reopen .heredoc");
////	if (dup2(heredoc_fd, STDIN_FILENO) == -1)
////	{
////		perror("dup2 error");
////		close(heredoc_fd);
////	}
////	close(heredoc_fd);
////	dup2(stdin_backup, STDIN_FILENO);
////	close(stdin_backup);
////	command->args[1] = ".heredoc";
////	 choose_command(command, env_list);
////	return ;
////}


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

