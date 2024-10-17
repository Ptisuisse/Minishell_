#include "minishell.h"

int	ft_is_valid(char **arg)
{
	int i;
	int j;
	int equal;

	i = 1;
	equal = 0;
	while (arg[i])
	{
		j = 0;
		while (arg[i][j])
		{
			if(arg[i][0] == '_')
			{
				if (arg[i][1] == '\0')
				{
					g_exit_code = 1;
					ft_printf("Minishell: export: `%s': not a valid identifier\n", arg[i]);
					return (0);
				}
			}
			else if (arg[i][j] == '=')
			{
				equal = 1;
				if (!ft_isalnum(arg[i][j - 1]))
				{
					g_exit_code = 1;
					ft_printf("Minishell: export: `%s': not a valid identifier\n", arg[i]);
					return (0);
				}
			}
			else if ((!ft_isalnum(arg[i][j]) || ft_isdigit(arg[i][0])) || arg[i][j] == '_')
			{
				if (equal == 1)
					g_exit_code = 0;
				else
				{
					g_exit_code = 1;
					ft_printf("Minishell: export: not a valid identifier\n");
				}
				return (0);
			}
			j++;
		}
		i++;
	}
	return (1);
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
