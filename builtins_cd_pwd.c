#include "minishell.h"

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
		ft_printf("Minishell: cd: too many arguments\n");
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
