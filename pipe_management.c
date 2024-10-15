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
	//while (wait(&status) > 0)
	//	;
	waitpid(commands->pid, &status, 0);
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
	else
		g_exit_code = 0;
	free(path);
	waitpid(pid, NULL, 0);
	return (1);
}
