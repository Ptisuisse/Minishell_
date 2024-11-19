/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsingutils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lvan-slu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/17 13:05:43 by lvan-slu          #+#    #+#             */
/*   Updated: 2024/10/17 13:05:43 by lvan-slu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_command	*init_command(int exit_code, t_env **env_list)
{
	t_command	*element;

	element = malloc(sizeof(t_command));
	if (element == NULL)
		return (NULL);
	element->output = 0;
	element->file = 0;
	element->status = 0;
	element->error_file = 0;
	element->exit_code = 0;
	element->save_in = 0;
	element->save_out = 0;
	element->error_message = NULL;
	element->output_file = NULL;
	element->input_file = NULL;
	element->append_file = NULL;
	element->heredoc_file = NULL;
	if (exit_code != 0)
		element->exit_code = exit_code;
	memset(element->args, 0, sizeof(element->args));
	element->next = NULL;
	element->prev = NULL;
	element->env = *env_list;
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

	if (!new)
		return ;
	if (!(*lst) || ((*lst)->next == NULL && (*lst)->prev == NULL && (*lst)->file == 0
		&& (*lst)->args[0] == NULL))
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
	bool	double_q;
	bool	sing_q;
	int		i;

	double_q = false;
	sing_q = false;
	i = 0;
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
