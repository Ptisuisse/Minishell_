#include "minishell.h"

void	start_builtins(t_command *command, t_env **env_list)
{
	if (!(ft_strcmp(command->args[0], "exit")))
		exit(0);
	else if (!(ft_strcmp(command->args[0], "echo")))
		echo_cmd(command->args, *env_list);
	else if (!(ft_strcmp(command->args[0], "cd")))
		cd_cmd(command->args[1]);
	else if (!(ft_strcmp(command->args[0], "pwd")))
		pwd_cmd();
	else if (!(ft_strcmp(command->args[0], "env")))
		env_cmd(*env_list);
	else if (!(ft_strcmp(command->args[0], "export")))
		*env_list = export_cmd(*env_list, command);
	else if (!(ft_strcmp(command->args[0], "unset")))
		unset_cmd(command->args[1], *env_list);
	else if (!(ft_strcmp(command->args[0], "clear")))
		clear_cmd();
}

void	clear_cmd(void)
{
	system("clear");
}

int	choose_command(t_command *command, t_env **env_list)
{
	int	result;

	result = -1;
	if (check_builtins(command, env_list))
		result = 0;
	else
	{
		exec_command(command->args[0], command->args);
		result = 0;
	}
	return (result);
}

int	pwd_cmd(void)
{
	char	cwd[1024];

	if (getcwd(cwd, sizeof(cwd)))
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

// int	echo_cmd(char **args, t_data *data)
// {
// 	int	newline;
// 	int	i;
// 	int	j;

// 	(void)data;
// 	newline = 1;
// 	i = 1;
// 	while (args[i])
// 	{
// 		if (args[i][0] == '-' && args[i][1] == 'n')
// 		{
// 			j = 1;
// 			while (args[i][j] == 'n')
// 				j++;
// 			if (args[i][j] == '\0')
// 			{
// 				newline = 0;
// 				i++;
// 			}
// 		}
// 		printf("%s", args[i]);
// 		if (args[i + 1] != NULL)
// 			printf(" ");
// 		i++;
// 	}
// 	if (newline)
// 		printf("\n");
// 	return (1);
// }

int echo_cmd(char **args, t_env *data)
{
    int newline;
    int i;

	newline = 1;
	i = 1;
	(void)data;
    while (args[i] && args[i][0] == '-' && args[i][1] == 'n')
	{
        int j = 1;
        while (args[i][j] == 'n')
            j++;
        if (args[i][j] == '\0')
		{
            newline = 0;
            i++;
        }
		else
            break;
    }
    while (args[i])
	{
        printf("%s", args[i]);
        i++;
        if (args[i])
            printf(" ");
    }
    if (newline)
        printf("\n");
    return 1;
}

int	cd_cmd(const char *path)
{
	if (chdir(path) != 0)
	{
		printf("Minishell: cd: %s: No such file or directory\n", path);
		return (0);
	}
	return (1);
}

