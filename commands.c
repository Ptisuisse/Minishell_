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

void	start_builtins(t_command *command, t_data *data)
{
	if (!(ft_strcmp(command->args[0], "exit")))
		exit(0);
	else if (!(ft_strcmp(command->args[0], "echo")))
		echo_cmd(command->args, data);
	else if (!(ft_strcmp(command->args[0], "cd")))
		cd_cmd(command->args[1]);
	else if (!(ft_strcmp(command->args[0], "pwd")))
		pwd_cmd();
	else if (!(ft_strcmp(command->args[0], "env")))
		env_cmd(data);
	else if (!(ft_strcmp(command->args[0], "export")))
		export_cmd(data, command);
	// envoyer la struct dans cette fonctionpour eviter pb i think
	else if (!(ft_strcmp(command->args[0], "unset")))
		unset_cmd(command->args[1], data);
	else if (!(ft_strcmp(command->args[0], "clear")))
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
	if (check_builtins(command, data))
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
int	echo_cmd(char **args, t_data *data)
{
	int	newline;
	int	i;
	int	j;

	(void)data;
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
		printf("Minishell: cd: %s: No such file or directory\n", path);
		return (0);
	}
	return (1);
}
// void	env_cmd(t_data *data)
//{
//	while (data->env_list)
//	{
//		printf("%s=%s\n", data->env_list->name, data->env_list->value);
//		data->env_list = data->env_list->next;
//	}
//}

void	env_cmd(t_data *data)
{
	t_env	*tmp;

	tmp = data->env_list;
	while (tmp)
	{
		printf("%s=%s\n", tmp->name, tmp->value);
		tmp = tmp->next;
	}
}

void	sorted_insert(t_env **head_ref, t_env *new_node)
{
	t_env	*current;

	if (*head_ref == NULL || ft_strcmp((*head_ref)->name, new_node->name) >= 0)
	{
		new_node->next = *head_ref;
		*head_ref = new_node;
	}
	else
	{
		current = *head_ref;
		while (current->next != NULL && ft_strcmp(current->next->name,
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

int	ft_is_valid(char *arg)
{
	int	i;

	i = 0;
	if (!ft_isalpha(arg[i]) && arg[i] != '_')
	{
		printf("Minishell: export: `%s': not a valid identifier\n", arg);
		return (1);
	}
	i++;
	while (arg[i])
	{
		if (!ft_isalnum(arg[i]) && arg[i] != '_')
		{
			printf("Minishell: export: `%s': not a valid identifier\n", arg);
			return (1);
		}
		i++;
	}
	return (0);
}

void	export_args(char *arg, t_env **env_list)
{
	t_env	*tmp;
	t_env	*new_node;
	char	*equal_sign;
	char	*name;
	char	*value;

	tmp = *env_list;
	equal_sign = strchr(arg, '=');
	if (equal_sign)
	{
		name = ft_substr(arg, 0, equal_sign - arg);
		value = ft_strdup(equal_sign + 1);
	}
	else
	{
		name = ft_strdup(arg);
		value = ft_strdup("");
	}
	while (tmp)
	{
		if (ft_strcmp(tmp->name, name) == 0)
		{
			if (equal_sign)
			{
				free(tmp->value);
				tmp->value = value;
			}
			free(name);
			return ;
		}
		tmp = tmp->next;
	}
	new_node = malloc(sizeof(t_env));
	if (!new_node)
	{
		perror("malloc");
		free(name);
		free(value);
		return ;
	}
	new_node->name = name;
	new_node->value = value;
	new_node->next = *env_list;
	*env_list = new_node;
}

int	export_cmd(t_data *data, t_command *command)
{
	t_data	*tmp;

	tmp = data;
	if (command->args[1] != NULL)
	{
		if (!ft_is_valid(command->args[1]))
			return (0);
		export_args(command->args[1], &data->env_list);
	}
	else
	{
		sort_env_list(&data->env_list);
		while (tmp->env_list)
		{
			if (tmp->env_list->name[0] == '_')
				tmp->env_list = tmp->env_list->next;
			printf("declare -x %s", tmp->env_list->name);
			if (tmp->env_list->value[0] != '\0')
				printf("=\"%s\"\n", tmp->env_list->value);
			else
				printf("\n");
			tmp->env_list = tmp->env_list->next;
		}
	}
	return (1);
}

void	unset_cmd(char *path, t_data *data)
{
	int		i;
	t_env	*tmp;
	t_env	*prev;

	i = 0;
	prev = NULL;
	tmp = data->env_list;
	while (tmp)
	{
		if (ft_strcmp(tmp->name, path) == 0)
		{
			if (prev == NULL)
				tmp = tmp->next;
			else
				prev->next = tmp->next;
		}
		prev = tmp;
		tmp = tmp->next;
	}
	data->env_list = tmp;
	printf_list(data->env_list);
}
