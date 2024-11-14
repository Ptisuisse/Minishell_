/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_redirections.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lisambet <lisambet@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/17 13:05:47 by lvan-slu          #+#    #+#             */
/*   Updated: 2024/11/14 12:29:29 by lisambet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	detect_invalid_double_redirection(const char **input, char **token)
{
	*token = NULL;
	if ((*(*input + 1) == '>' || *(*input + 1) == '<') || (*(*input + 2) == '>'
			|| *(*input + 2) == '<'))
	{
		if (*(*input + 2) == '<' && *(*input + 3) != '\0')
		{
			*token = "newline";
			if (*(*input + 2) == '<')
				*token = "<";
			if (*(*input + 3) == '<')
				*token = "<";
			if (*(*input + 3) == '>')
				*token = ">";
			if (*(*input + 4) == '<')
				*token = "<<";
		}
		else if (*(*input + 2) == '>')
		{
			*token = ">";
			if (*(*input + 3) == '>')
				*token = ">>";
		}
		else if (!ft_isascii(*(*input + 2)) && (*(*input + 3) == '\0'
				|| !*(*input + 3)))
			*token = "newline";
		return (*token != NULL);
	}
	return (0);
}

//	else if (*(*input + 3) == '\0' && *(*input + 2) == '>')

int	handle_redirection_error(const char *token, t_command *cmd)
{
	if (token)
	{
		error_message(token, cmd);
		return (1);
	}
	return (0);
}

int	check_double_redirection(const char **input, t_command *cmd)
{
	char	*token;

	if (detect_invalid_double_redirection(input, &token))
		return (handle_redirection_error(token, cmd));
	return (0);
}

int	handle_redirection(const char **input, t_command *cmd)
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

int	handle_redirection_and_arguments(const char **input, t_command *cmd,
		int *arg_index)
{
	if (**input == '<' || **input == '>')
	{
		if (handle_redirection(input, cmd))
			return (1);
	}
	else if ((**input == '"' && *(*input + 1) == '.') || (**input == '.'
			|| **input == '/'))
	{
		parse_arguments(input, cmd, arg_index);
		just_a_path(cmd); //./libft/ft_atoi.c echo
		return (0);
	}
	else
		parse_arguments(input, cmd, arg_index);
	return (0);
}
