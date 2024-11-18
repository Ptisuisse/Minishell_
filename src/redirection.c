/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lvan-slu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/17 13:05:47 by lvan-slu          #+#    #+#             */
/*   Updated: 2024/10/17 13:05:50 by lvan-slu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	handle_input_redirection(const char **input, t_command **cmd)
{
	char	buffer[1024];
	int		buf_index;
	int		status;

	status = 0;
	buf_index = 0;
	(*input)++;
	(*cmd)->file++;
	if (**input == '<')
	{
		(*input)++;
		while (**input == ' ')
			(*input)++;
		parse_argument(input, buffer, &buf_index, cmd);
		(*cmd)->heredoc_file = ft_strdup(buffer);
	}
	else
	{
		while (**input == ' ')
			(*input)++;
		parse_argument(input, buffer, &buf_index, cmd);
		if (ft_strchr(buffer, '<'))
		{
			error_message("<<", cmd);
			return (1);
		}
		(*cmd)->input_file = ft_strdup(buffer);
		status = parsing_error_inputfile(*cmd, (*cmd)->input_file);
		if (status > 0)
		{
			advance_to_end_or_pipe(input);
			if ((*cmd)->append_file != NULL)
				(*cmd)->append_file = NULL;
			if ((*cmd)->output_file != NULL)
				(*cmd)->output_file = NULL;
		}
	}
	return (0);
}

int	handle_output_redirection(const char **input, t_command **cmd)
{
	char	buffer[1024];
	int		buf_index;
	int		status;

	(*cmd)->file++;
	status = 0;
	buf_index = 0;
	(*input)++;
	if (**input == '>')
	{
		(*input)++;
		while (**input == ' ')
			(*input)++;
		parse_argument(input, buffer, &buf_index, cmd);
		(*cmd)->append_file = ft_strdup(buffer);
		status = parsing_error_appendfile(*cmd);
		if (status > 0)
		{
			advance_to_end_or_pipe(input);
			return (status);
		}
		else
			open((*cmd)->append_file, O_WRONLY | O_CREAT | O_APPEND, 0644);
		(*cmd)->output = 3;
	}
	else
	{
		while (**input == ' ')
			(*input)++;
		parse_argument(input, buffer, &buf_index, cmd);
		if (ft_strchr(buffer, '>'))
		{
			error_message(">>", cmd);
			return (1);
		}
		(*cmd)->output_file = ft_strdup(buffer);
		status = parsing_error_outputfile(*cmd);
		if (status > 0)
		{
			advance_to_end_or_pipe(input);
			return (status);
		}
		else
			open((*cmd)->output_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
		(*cmd)->output = 4;
	}
	return (0);
}

void	advance_to_end_or_pipe(const char **input)
{
	while (**input != '\0' && **input != '|')
		(*input)++;
}

int	parse_redirection(const char **input, t_command **cmd)
{
	int result;

	result = 0;
	if (**input == '<')
		handle_input_redirection(input, cmd);
	else if (**input == '>')
		handle_output_redirection(input, cmd);
	return (result);
}