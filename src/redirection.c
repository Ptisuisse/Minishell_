/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lisambet <lisambet@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/17 13:05:47 by lvan-slu          #+#    #+#             */
/*   Updated: 2024/11/19 17:26:37 by lisambet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	handle_input_file(const char **input, t_command *cmd, char *buffer)
{
	int	buf_index;
	int	status;

	buf_index = 0;
	while (**input == ' ')
		(*input)++;
	parse_argument(input, buffer, &buf_index, &cmd);
	if (ft_strchr(buffer, '<'))
	{
		error_message("<<", &cmd);
		return (1);
	}
	cmd->input_file = ft_strdup(buffer);
	status = parsing_error_inputfile(cmd, cmd->input_file);
	if (status > 0)
	{
		advance_to_end_or_pipe(input);
		cmd->append_file = NULL;
		cmd->output_file = NULL;
	}
	return (status);
}

int	handle_input_redirection(const char **input, t_command **cmd)
{
	char	buffer[1024];

	(*input)++;
	(*cmd)->file++;
	if (**input == '<')
		return (handle_heredoc(input, *cmd, buffer));
	else
		return (handle_input_file(input, *cmd, buffer));
}

int	handle_append_redirection(const char **input, t_command *cmd, char *buffer)
{
	int	buf_index;
	int	status;

	buf_index = 0;
	(*input)++;
	while (**input == ' ')
		(*input)++;
	parse_argument(input, buffer, &buf_index, &cmd);
	cmd->append_file = ft_strdup(buffer);
	status = parsing_error_appendfile(cmd);
	if (status > 0)
	{
		advance_to_end_or_pipe(input);
		return (status);
	}
	//open(cmd->append_file, O_WRONLY | O_CREAT | O_APPEND, 0644);
	cmd->output = 3;
	return (0);
}

int	handle_trunc_redirection(const char **input, t_command *cmd, char *buffer)
{
	int	buf_index;
	int	status;

	buf_index = 0;
	while (**input == ' ')
		(*input)++;
	parse_argument(input, buffer, &buf_index, &cmd);
	if (ft_strchr(buffer, '>'))
	{
		error_message(">>", &cmd);
		return (1);
	}
	cmd->output_file = ft_strdup(buffer);
	status = parsing_error_outputfile(cmd);
	if (status > 0)
	{
		advance_to_end_or_pipe(input);
		return (status);
	}
	open(cmd->output_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	cmd->output = 4;
	return (0);
}

int	handle_output_redirection(const char **input, t_command **cmd)
{
	char	buffer[1024];

	(*cmd)->file++;
	(*input)++;
	if (**input == '>')
		return (handle_append_redirection(input, *cmd, buffer));
	else
		return (handle_trunc_redirection(input, *cmd, buffer));
}
