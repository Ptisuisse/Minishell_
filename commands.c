#include "minishell.h"

void	start_builtins(t_command *command, t_env **env_list)
{
	if (!(ft_strcmp(command->args[0], "exit")))
		exit_cmd(ft_atoi(command->args[1]));
	else if (!(ft_strcmp(command->args[0], "echo")))
		echo_cmd(command->args);
	else if (!(ft_strcmp(command->args[0], "cd")))
		cd_cmd(command->args[1], *env_list);
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

int	exit_cmd(int args)
{
	exit(args);
}

int	pwd_cmd(void)
{
	char	*cwd;

	cwd = getcwd(NULL, 0);
	if (cwd)
	{
		printf("%s\n", cwd);
		free(cwd);
		return (1);
	}
	else
	{
		printf("Error : pwd\n");
		return (0);
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
	exit_cmd(0);
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

int	cd_cmd(const char *path, t_env *env_list)
{
	char	*oldpwd;
	char	*newpwd;

	oldpwd = getcwd(NULL, 0);
	if (!path)
		path = ft_strdup(getenv("HOME"));
	if (chdir(path) < 0)
	{
		printf("Minishell: cd: %s: No such file or directory\n", path);
		free(oldpwd);
		return (0);
	}
	newpwd = getcwd(NULL, 0);
	update_env(env_list, "PWD", newpwd);
	update_env(env_list, "OLDPWD", oldpwd);
	free(oldpwd);
	free(newpwd);
	return (1);
}

