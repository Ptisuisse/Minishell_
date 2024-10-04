#include "minishell.h"

void	test_pipe(t_command *commands, t_env **env_list)
{
	if (commands->next != NULL)
		commands_manager(commands, env_list);
	else
		choose_command(commands, env_list);
}

void	commands_manager(t_command *commands, t_env **env_list)
{
	t_command	*cmd;
	int			status;

	cmd = commands;
	if (commands->input_fd == 1)
		redirect_input(commands);
	if (commands->output_fd == 1)
		redirect_output(commands);
	while (commands)
	{
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
	while (wait(&status) > 0)
		;
}

int	exec_command(char *pathname, char **args)
{
	char *path;
	int pid;

	pid = fork();
	path = ft_strjoin("/bin/", pathname); //creer une fonction savoir si path valide ou non car faire /bin/ls dans le terminal ne marche pas
	if (pid == 0)
	{
		if (execve(path, args, NULL) == -1)
		{
			printf("%s: command not found\n", pathname);
			free(path);
			return (0);
		}
	}
	waitpid(pid, NULL, 0);
	free(path);
	return (1);
}