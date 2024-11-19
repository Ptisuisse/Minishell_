/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_redirections.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lisambet <lisambet@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/17 13:05:47 by lvan-slu          #+#    #+#             */
/*   Updated: 2024/11/19 17:27:30 by lisambet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	handle_redirection_error(const char *token, t_command **cmd)
{
	if (token)
	{
		error_message(token, cmd);
		return (1);
	}
	return (0);
}

int	handle_redirection(const char **input, t_command **cmd)
{
	char	*token;

	token = NULL;
	if (check_double_redirection(input, cmd))
		return (1);
	if (*(*input + 1) && *(*input + 1) != ' ' && !ft_isascii(*(*input + 1)))
	{
		token = "|";
		error_message(token, cmd);
		return (1);
	}
	else if (*(*input + 1) == '\0' || *(*input + 1) == '|')
	{
		token = "newline";
		error_message(token, cmd);
		return (1);
	}
	return (parse_redirection(input, cmd));
}

int	handle_redirection_and_arguments(const char **input, t_command **cmd,
		int *arg_index)
{
	if (**input == '<' || **input == '>')
	{
		if (handle_redirection(input, cmd))
			return (1);
	}
	else
		parse_arguments(input, cmd, arg_index);
	return (0);
}

void	advance_to_end_or_pipe(const char **input)
{
	while (**input != '\0' && **input != '|')
		(*input)++;
}

int	parse_redirection(const char **input, t_command **cmd)
{
	int	result;

	result = 0;
	if (**input == '<')
		handle_input_redirection(input, cmd);
	else if (**input == '>')
		handle_output_redirection(input, cmd);
	return (result);
}
