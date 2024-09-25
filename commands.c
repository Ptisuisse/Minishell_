#include "minishell.h"

//void	command_manager(t_command *command_list, t_data data)
//{
//	int	exec;

//	while (command_list)
//	{
//		exec = select_commands(command_list, data);
//		if (exec == -1)
//			exit(1);
//		command_list = command_list->next;
//	}
//	free_commands(command_list);
//}

void	start_builtins(char **command, t_data data)
{
	if (ft_strcmp(command[0], "exit"))
		exit(0);
	else if (ft_strcmp(command[0], "echo"))
		echo_cmd(command);
	else if (ft_strcmp(command[0], "cd"))
		cd_cmd(command[1]);
	else if (ft_strcmp(command[0], "pwd"))
		pwd_cmd();
	else if (ft_strcmp(command[0], "env"))
		env_cmd(data);
}

int	choose_command(t_command *command, t_data data)
{
	int	result;

	result = -1;
	if (check_builtins(command->args, data))
		result = 0;
	else if (execute_command(command->args[0], command->args))
		result = 0;
	return (result);
}

int	execute_command(char *pathname, char **args)
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

void	env_cmd(t_data data)
{
	int	i;

	i = 0;
	while (data.env[i])
	{
		printf("%s\n", data.env[i]);
		i++;
	}
}

int	echo_cmd(char **args)
{
	int	newline;
	int	i;

	newline = 1;
	i = 1;
	while (args[i])
	{
		if (ft_strcmp(args[i], "-n")) // ft_strcmp
		{
			newline = 0;
			i++;
		}
		printf("%s", args[i]);
		if (args[i + 1] != NULL)
			printf(" ");
		i++;
	}
	if (newline)
		printf("\n");
	return (1);
}

int	cd_cmd(char *path)
{
	if (chdir(path) != 0)
	{
		printf("Error : cd\n");
		return (0);
	}
	return (1);
}

int	pwd_cmd(void)
{
	char	cwd[1024];

	if (getcwd(cwd, sizeof(cwd)) != NULL)
	{
		printf("%s\n", cwd);
		return (1);
	}
	else
	{
		printf("Error : pwd\n");
		return (0);
	}
}
