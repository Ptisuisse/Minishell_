#include "minishell.h"

void	create_env_list(t_data *data)
{
	int		i;
	t_env	*new;
	t_env	*tmp;
	char	*equal_sign;

	i = 0;
	while (data->env[i])
	{
		new = (t_env *)malloc(sizeof(t_env));
		if (!new)
			return ;
		new->next = NULL;
		equal_sign = ft_strchr(data->env[i], '=');
		if (equal_sign)
		{
			new->name = ft_substr(data->env[i], 0, equal_sign - data->env[i]);
			equal_sign++;
			new->value = ft_strdup(equal_sign);
		}
		else
		{
			new->name = ft_strdup(data->env[i]);
			new->value = NULL;
		}
		if (i == 0)
			data->env_list = new;
		else
		{
			tmp = data->env_list;
			while (tmp->next)
				tmp = tmp->next;
			tmp->next = new;
		}
		i++;
	}
}

void	start_builtins(char **command, t_data *data)
{
	if (!ft_strcmp(command[0], "exit"))
		exit(0);
	else if (!ft_strcmp(command[0], "echo"))
		echo_cmd(command, data);
	else if (!ft_strcmp(command[0], "cd"))
		cd_cmd(command[1]);
	else if (!ft_strcmp(command[0], "pwd"))
		pwd_cmd();
	else if (!ft_strcmp(command[0], "env"))
		env_cmd(data);
	else if (!ft_strcmp(command[0], "export"))
		export_cmd(data);
	else if (!ft_strcmp(command[0], "clear"))
		clear_cmd();
}

void	clear_cmd(void)
{
	system("clear");
}

int	choose_command(t_command *command, t_data *data)
{
	int	result;

	result = -1;
	if (check_builtins(command->args, data))
		result = 0;
	else
	{
		exec_command(command->args[0], command->args);
		result = 0;
	}
	return (result);
}

void	env_cmd(t_data *data)
{
	t_env	*tmp;
	int		i;

	i = 0;
	tmp = data->env_list;
	while (tmp)
	{
		i++;
		tmp = tmp->next;
	}
}

int	echo_cmd(char **args, t_data *data)
{
	t_env	*env;
	int		newline;
	int		i;
	int		j;

	(void)args;
	newline = 1;
	i = 1;
	while (args[i])
	{
		if (args[i][0] == '-' && args[i][1] == 'n')
		{
			j = 2;
			while (args[i][j] == 'n')
				j++;
			if (args[i][j] == '\0')
			{
				newline = 0;
				i++;
			}
		}
		if (args[i][0] == '$') // ----> voir pour gerer autrement
		{
			env = data->env_list;
			while (env)
			{
				if (ft_strcmp(env->name, args[i] + 1) == 0)
				{
					printf("%s", env->value);
					break ;
				}
				env = env->next;
			}
		}
		else
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
void	sorted_insert(t_env **head_ref, t_env *new_node)
{
	t_env	*current;

	if (*head_ref == NULL || strcmp((*head_ref)->name, new_node->name) >= 0)
	{
		new_node->next = *head_ref;
		*head_ref = new_node;
	}
	else
	{
		current = *head_ref;
		while (current->next != NULL && strcmp(current->next->name,
				new_node->name) < 0)
		{
			current = current->next;
		}
		new_node->next = current->next;
		current->next = new_node;
	}
}

void	sort_env_list(t_env **head_ref)
{
	t_env	*sorted;
	t_env	*current;
	t_env	*next;

	sorted = NULL;
	current = *head_ref;
	while (current != NULL)
	{
		next = current->next;
		sorted_insert(&sorted, current);
		current = next;
	}
	*head_ref = sorted;
}

int	export_cmd(t_data *data)
{
	sort_env_list(&data->env_list);
	while (data->env_list)
	{
		printf("declare -x %s=\"%s\"\n", data->env_list->name,
			data->env_list->value);
		data->env_list = data->env_list->next;
	}
	return (1);
}
