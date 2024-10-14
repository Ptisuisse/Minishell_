#include "minishell.h"

t_command	*init_command(void)
{
	t_command	*element;

	element = malloc(sizeof(t_command));
	if (element == NULL)
		return (NULL);
	element->input_file = NULL;
	element->output_file = NULL;
	element->append_file = NULL;
	element->exit_code = 0;
	memset(element->args, 0, sizeof(element->args));
	element->next = NULL;
	element->prev = NULL;
	return (element);
}

t_command	*ft_lstlst(t_command *lst)
{
	if (lst == NULL)
		return (NULL);
	while (lst->next)
		lst = lst->next;
	return (lst);
}

void	append_command_node(t_command **lst, t_command *new)
{
	t_command	*tmp;

	if (!new || !lst)
		return ;
	if (*lst == NULL)
		*lst = new;
	else
	{
		tmp = ft_lstlst(*lst);
		tmp->next = new;
		new->prev = tmp;
	}
	return ;
}

void	quoting_choice(bool *double_q, bool *sing_q, int *index, char c)
{
	if ((c == '\'' || c == '"') && !*sing_q && !*double_q)
	{
		if (c == '\'' && !*double_q)
			*sing_q = true;
		else if (c == '"' && !*sing_q)
			*double_q = true;
		if (index)
			++(*index);
	}
	else if ((c == '\'' || c == '"'))
	{
		if (c == '\'' && !*double_q && *sing_q)
			*sing_q = false;
		else if (c == '"' && !*sing_q && *double_q)
			*double_q = false;
		if (index)
			++(*index);
	}
}

int	open_quote(const char *line)
{
	bool double_q = false;
	bool sing_q = false;
	int i = 0;

	while (line && line[i])
	{
		quoting_choice(&double_q, &sing_q, &i, line[i]);
		if (line[i] && line[i] != '\'' && line[i] != '"')
			++i;
	}
	if (double_q || sing_q)
		return (1);
	return (0);
}