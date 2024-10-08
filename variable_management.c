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


void	env_cmd(t_env *env_list)
{
	t_env	*head;

	head = env_list;
	while (env_list)
	{
		if (!env_list->value)
			env_list = env_list->next;
		else
		{
			printf("%s=%s\n", env_list->name, env_list->value);
			env_list = env_list->next;
		}
	}
	env_list = head;
}

void	sorted_insert(t_env **head_ref, t_env *new_node)
{
	t_env	*current;

	if (*head_ref == NULL || ft_strcmp((*head_ref)->name, new_node->name) > 0)
	{
		new_node->next = *head_ref;
		*head_ref = new_node;
	}
	else
	{
		current = *head_ref;
		while (current->next != NULL && ft_strcmp(current->next->name, new_node->name) <= 0)
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

t_env	*export_args(char *arg, t_env *env_list)
{
	t_env	*tmp;
	t_env	*new_node;
	char	*equal_sign;
	char	*name;
	char	*value;

	tmp = env_list;
	equal_sign = ft_strchr(arg, '=');
	if (equal_sign)
	{
		name = ft_substr(arg, 0, equal_sign - arg);
		value = ft_strdup(equal_sign + 1);
	}
	else
	{
		name = ft_strdup(arg);
		value = NULL;
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
			return(env_list);
		}
		tmp = tmp->next;
	}
	new_node = malloc(sizeof(t_env));
	if (!new_node)
	{
		perror("malloc");
		free(name);
		free(value);
		return (NULL);
	}
	new_node->name = name;
	new_node->value = value;
	new_node->next = env_list;
	env_list = new_node;
	return (env_list);
}

int	ft_is_valid(char **arg)
{
	int i;
	int j;

	i = 1;
	while (arg[i])
	{
		j = 0;
		while (arg[i][j])
		{
			if(arg[i][0] == '_')
			{
				if (arg[i][1] == '\0')
				{
					printf("Minishell: export: `%s': not a valid identifier\n", arg[i]);
					return (0);
				}
			}
			else if (arg[i][j] == '=')
			{
				if (!ft_isalnum(arg[i][j - 1]))
				{
					printf("Minishell: export: `%s': not a valid identifier\n", arg[i]);
					return (0);
				}
			}
			else if ((!ft_isalnum(arg[i][j]) || ft_isdigit(arg[i][0])) || arg[i][j] == '_')
			{
				printf("Minishell: export: `%s': not a valid identifier\n", arg[i]);
				return (0);
			}
			j++;
		}
		i++;
	}
	return (1);
}

t_env	*export_cmd(t_env *env_list, t_command *command)
{
	t_env	*head;

	head = env_list;
	if (command->args[1] != NULL)
	{
		if (!ft_is_valid(command->args))
			return (head);
		env_list = export_args(command->args[1], env_list);
		if (env_list != NULL)
			head = env_list;
	}
	else
	{
		sort_env_list(&head);
		t_env *current = head;
		while (current)
		{
			if (current->name[0] == '_' && current->name[1] == '\0')
			{
				if (current->next)
					current = current->next;
				else
					break ;
			}
			if (current->value == NULL)
				printf("declare -x %s\n", current->name);
			else
				printf("declare -x %s=\"%s\"\n", current->name, current->value);
			current = current->next;
		}
	}
	return (head);
}

void	unset_cmd(char *path, t_env *env_list)
{
	t_env	*head;
	t_env	*prev;

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
	env_list = prev;
}