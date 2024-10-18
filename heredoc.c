/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lvan-slu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/17 15:11:58 by lvan-slu          #+#    #+#             */
/*   Updated: 2024/10/17 15:11:59 by lvan-slu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	go_heredoc(t_command *command, t_env *env_lst, int fd_doc)
{
	char	*input;
	char *oper_value;
	char	*end_of_input;

	input = NULL;
	if (command->append_outfd == 1)
		end_of_input = command->append_outfile;
	while (1)
	{
		input = readline("> ");
		if (strcmp(input, end_of_input) == 0)
			break;
		ft_putstr_fd(input, fd_doc);
		ft_putstr_fd("\n", fd_doc);
		free(input);
	}
	if (input != 0)
		free(input);
}

static void	child_heredoc(t_command *command, t_env **env_list, int *heredoc)
{
	signal(SIGINT, handle_signal);
	close(heredoc[0]);
	go_heredoc(command, *env_list, heredoc[1]);
	close(heredoc[1]);
	exit(0);
}

static int	parent_heredoc(t_command *command, int *heredoc)
{
    int exit_status;
    int std_in;

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

int	heredoc(t_command *command, t_env **env_list)
{
	int	pid;
	int	heredoc[2];
	int	i;

	i = pipe(heredoc);
	if (i == -1)
		exit(1);
	pid = fork();
	if (pid == 0)
		child_heredoc(command, env_list, heredoc);
	else
		return (parent_heredoc(command, heredoc));
	return (0);
}
