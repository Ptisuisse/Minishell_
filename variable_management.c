#include "minishell.h"


void	create_env_list(char **envp, t_env **env_list)
{
	int		i;
	t_env	*new;
	t_env	*tmp;
	char	*equal_sign;

	i = 0;
	while (envp[i])
	{
		new = malloc(sizeof(t_env));
		if (!new)
			return ;
		new->next = NULL;
		equal_sign = ft_strchr(envp[i], '=');
		if (equal_sign)
		{
			new->name = ft_substr(envp[i], 0, equal_sign - envp[i]);
			equal_sign++;
			new->value = ft_strdup(equal_sign);
		}
		else
		{
			new->name = ft_strdup(envp[i]);
			new->value = NULL;
		}
		if (i == 0)
			*env_list = new;
		else
		{
			tmp = *env_list;
			while (tmp->next)
				tmp = tmp->next;
			tmp->next = new;
		}
		i++;
	}
}


void	env_cmd(t_command *command, t_env *env_list)
{
	t_env	*head;
	char	*equal_sign;

	head = env_list;
	equal_sign = ft_strchr(command->args[1], '=');
	while (env_list)
	{
		// if (equal_sign != NULL)
		// 	env_list = env_list->next;
		//else
			printf("%s=%s\n", env_list->name, env_list->value);
		env_list = env_list->next;
	}
	command = command->next;
	env_list = head;
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

t_env	*export_args(char *arg, t_env *env_list)
{
	t_env	*tmp;
	t_env	*new_node;
	char	*equal_sign;
	char	*name;
	char	*value;

	tmp = env_list;
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
			return(NULL);
		}
		tmp = tmp->next;
	}
	new_node = malloc(sizeof(t_env));
	if (!new_node)
	{
		perror("malloc");
		free(name);
		free(value);
		return(NULL);
	}
	new_node->name = name;
	new_node->value = value;
	new_node->next = env_list;
	env_list = new_node;
	return (env_list);
}

t_env	*export_cmd(t_env *env_list, t_command *command)
{
	t_env	*head;

	head = env_list;
	if (command->args[1] != NULL)
	{
		if (ft_is_valid(command->args[1]))
			return (NULL);
		env_list = export_args(command->args[1], env_list);
	}
	else
	{
		sort_env_list(&env_list);
		while (env_list)
		{
			if (env_list->name[0] == '_')
				env_list = env_list->next;
			printf("declare -x %s", env_list->name);
			if (env_list->value[0] != '\0')
				printf("=\"%s\"\n", env_list->value);
			else
				printf("\n");
			env_list = env_list->next;
		}
	}
	head = env_list;	
	return (env_list);
}

void	unset_cmd(char *path, t_env *env_list)
{
	//int		i;
	t_env	*head;
	t_env	*prev;

	//i = 0;
	prev = NULL;
	head = env_list;
	while (env_list)
	{
		if (ft_strcmp(env_list->name, path) == 0)
		{
			if (prev == NULL)
				env_list = env_list->next;
			else
				prev->next = env_list->next;
		}
		prev = env_list;
		env_list = env_list->next;
	}
	env_list = head;
	//printf_list(env_list);
}