/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_redirections.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lvan-slu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/17 13:05:47 by lvan-slu          #+#    #+#             */
/*   Updated: 2024/11/06 11:52:50 by lvan-slu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	handle_input_redirection(const char **input, t_command *cmd)
{
	char	buffer[1024];
	int		buf_index;

	buf_index = 0;
	(*input)++;
	cmd->file++;
	if (**input == '<')
	{
		(*input)++;
		while (**input == ' ')
			(*input)++;
		parse_argument(input, buffer, &buf_index, cmd);
		cmd->heredoc_file = strdup(buffer);
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
		cmd->input_file = ft_strdup(buffer);
		if (!parsing_error_inputfile(cmd))
		{
			cmd->error_file = 1;
			while (**input)
			{
				if (**input == '|')
					break ;
				(*input)++;
			}
			return (0);
		}
	}
	return (0);
}

int	handle_output_redirection(const char **input, t_command *cmd)
{
	char	buffer[1024];
	int		buf_index;

	buf_index = 0;
	(*input)++;
	cmd->file++;
	if (**input == '>')
	{
		(*input)++;
		while (**input == ' ')
			(*input)++;
		parse_argument(input, buffer, &buf_index, cmd);
		cmd->append_file = strdup(buffer);
		if (!parsing_error_outputfile(cmd))
		{
			cmd->error_file = 1;
			return (0);
		}
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
		cmd->output_file = strdup(buffer);
		if (!parsing_error_outputfile(cmd))
		{
			cmd->error_file = 1;
			return (0);
		}
	}
	return (0);
}

void	advance_to_end_or_pipe(const char **input)
{
	while (**input != '\0' && **input != '|')
		(*input)++;
}

int	parse_redirection(const char **input, t_command *cmd)
{
	int	result;

	result = 0;
	if (**input == '<')
		result = handle_input_redirection(input, cmd);
	else if (**input == '>')
		result = handle_output_redirection(input, cmd);
	return (result);
}
