#include "minishell.h"

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
