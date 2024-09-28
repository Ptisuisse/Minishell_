#include "minishell.h"

void commands_manager(t_command *commands, t_data *data)
{
    t_command *cmd = commands;
    int prev_pipe_read = -1;
    int status;

    while (cmd)
	{
        if (cmd->next)
            pipe(cmd->pipe);
        cmd->pid = fork();
		if (cmd->pid == 0)
		{
            if (prev_pipe_read != -1)
			{
                dup2(prev_pipe_read, STDIN_FILENO);
                close(prev_pipe_read);
            }
            if (cmd->next)
			{
                close(cmd->pipe[READ_END]);
                dup2(cmd->pipe[WRITE_END], STDOUT_FILENO);
                close(cmd->pipe[WRITE_END]);
            }
			choose_command(cmd, data);
            exit(EXIT_FAILURE);
        }
		else
		{
            if (prev_pipe_read != -1)
                close(prev_pipe_read);
            if (cmd->next)
			{
                close(cmd->pipe[WRITE_END]);
                prev_pipe_read = cmd->pipe[READ_END];
            }
        	cmd = cmd->next;
        }
	}
    while (wait(&status) > 0);
}


int	exec_command(char *pathname, char **args)
{
	char	*path;
	int		pid;

	pid = fork();
	path = ft_strjoin("/bin/", pathname);
	if (pid == 0)
	{
		if (execve(path, args, NULL) == -1)
		{
			printf("execve\n");
			free(path);
			return (0);
		}
	}
	waitpid(pid, NULL, 0);
	free(path);
	return (1);
}