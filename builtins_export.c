#include "minishell.h"

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

t_env	*export_cmd(t_env *env_list, t_command *command)
{
	t_env	*head;
	t_env	*current;

	current = NULL;
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
		current = head;
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
