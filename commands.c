#include "minishell.h"

void	start_builtins(t_command *command, t_env **env_list)
{
	if (!(ft_strcmp(command->args[0], "exit")))
		exit_cmd(command);
	else if (!(ft_strcmp(command->args[0], "echo")))
		echo_cmd(command->args);
	else if (!(ft_strcmp(command->args[0], "cd")))
		cd_cmd(command, *env_list);
	else if (!(ft_strcmp(command->args[0], "pwd")))
		pwd_cmd();
	else if (!(ft_strcmp(command->args[0], "env")))
		env_cmd(*env_list);
	else if (!(ft_strcmp(command->args[0], "export")))
		*env_list = export_cmd(*env_list, command);
	else if (!(ft_strcmp(command->args[0], "unset")))
		unset_cmd(command, *env_list);
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

int	exit_cmd(t_command *command)
{
	if (ft_strcmp(command->args[0], "exit") == 0 && command->args[2])
	{
		ft_printf(" too many arguments\n");
		g_exit_code = 1;
	}
	else if (ft_strcmp(command->args[0], "exit") == 0 && command->args[1])
	{
		if (ft_isdigit(command->args[1][0]))
			g_exit_code = ft_atoi(command->args[1]);
		else if (command->args[1][0] == '-')
			g_exit_code = 156;
		else if (ft_isalpha(command->args[1][0]))
		{
			ft_printf(" numeric argument required\n");
			g_exit_code = 2;
		}
		else
			g_exit_code = 255;
	}
	exit(g_exit_code);
}

void	pwd_cmd(void)
{
	char	*cwd;

	cwd = getcwd(NULL, 0);
	if (cwd)
	{
		printf("%s\n", cwd);
		free(cwd);
		g_exit_code = 0;
	}
	else
	{
		ft_printf("Error : pwd\n");
		g_exit_code = 1;
	}
}

void echo_cmd(char **args)
{
	int newline;
	int i;
	int	j;

	newline = 1;
	i = 1;
	while (args[i] && args[i][0] == '-')
	{
		j = 1;
		while (args[i][j] == 'n')
			j++;
		if (args[i][j] == '\0')
			newline = 0;
		else
			break;
		i++;
	}
	while (args[i])
	{
		printf("%s", args[i++]);
		if (args[i])
			printf(" ");
	}
	if (newline)
		printf("\n");
	g_exit_code = 0;
}

//int echo_cmd(char **args, t_env *data)
//{
//	int newline;
//	int i;

//	newline = 1;
//	i = 1;
//	(void)data;
//	while (args[i] && args[i][0] == '-' && args[i][1] == 'n')
//	{
//		int j = 1;
//		while (args[i][j] == 'n')
//			j++;
//		if (args[i][j] == '\0')
//		{
//			newline = 0;
//			i++;
//		}
//		else
//			break;
//	}
//	while (args[i])
//	{
//		printf("%s", args[i]);
//		i++;
//		if (args[i])
//			printf(" ");
//	}
//	if (newline)
//		printf("\n");
//	return 1;
//}

void	update_env(t_env *env_list, char *name, char *value)
{
	t_env	*head;

	head = env_list;
	while (env_list)
	{
		if (!(ft_strcmp(env_list->name, name)))
		{
			free(env_list->value);
			env_list->value = ft_strdup(value);
			break ;
		}
		env_list = env_list->next;
	}
	env_list = head;
}

void	cd_cmd(t_command *command, t_env *env_list)
{
	char	*oldpwd;
	char	*newpwd;

	oldpwd = getcwd(NULL, 0);
	if (!command->args[1])
		command->args[1] = ft_strdup(getenv("HOME"));
	if (command->args[2])
	{
				ft_printf("Minishell: cd: %s too many arguments\n", command->args[1]);
		g_exit_code = 1;
	}
	else if (chdir(command->args[1]) < 0)
	{
		ft_printf("Minishell: cd: %s: No such file or directory\n", command->args[1]);
		g_exit_code = 1;
	}
	else
		g_exit_code = 0;
	newpwd = getcwd(NULL, 0);
	update_env(env_list, "PWD", newpwd);
	update_env(env_list, "OLDPWD", oldpwd);
	free(oldpwd);
	free(newpwd);
}

