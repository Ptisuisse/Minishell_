/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_redirections.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lisambet <lisambet@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/19 17:15:26 by lisambet          #+#    #+#             */
/*   Updated: 2024/11/19 17:17:26 by lisambet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	check_triple_redirect(const char **input, char **token)
{
	if (*(*input + 2) == '<' && *(*input + 3) != '\0')
	{
		*token = "newline";
		if (*(*input + 3) == '<')
			*token = "<";
		else if (*(*input + 3) == '>')
			*token = ">";
		else if (*(*input + 4) == '<')
			*token = "<<";
		return (1);
	}
	return (0);
}

static int	check_double_redirect(const char **input, char **token)
{
	if (*(*input + 2) == '>')
	{
		*token = ">";
		if (*(*input + 3) == '>')
			*token = ">>";
		return (1);
	}
	return (0);
}

static int	check_non_ascii_redirect(const char **input, char **token)
{
	if (!ft_isascii(*(*input + 2)) && (*(*input + 3) == '\0' || !*(*input + 3)))
	{
		*token = "newline";
		return (1);
	}
	return (0);
}

int	detect_invalid_double_redirection(const char **input, char **token)
{
	*token = NULL;
	if ((*(*input + 1) == '>' || *(*input + 1) == '<') || (*(*input + 2) == '>'
			|| *(*input + 2) == '<'))
	{
		if (check_triple_redirect(input, token) || check_double_redirect(input,
				token) || check_non_ascii_redirect(input, token))
		{
			return (1);
		}
	}
	return (0);
}

int	check_double_redirection(const char **input, t_command **cmd)
{
	char	*token;

	if (detect_invalid_double_redirection(input, &token))
		return (handle_redirection_error(token, cmd));
	return (0);
}
