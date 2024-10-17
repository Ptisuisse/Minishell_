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

void	unset_cmd(t_command *command, t_env *env_list)
{
	t_env	*prev;

	prev = NULL;
	if (command->args[1] == NULL)
	{
		g_exit_code = 0;
		return ;
	}
	while (env_list)
	{
		if (ft_strcmp(env_list->name, command->args[1]) == 0)
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
