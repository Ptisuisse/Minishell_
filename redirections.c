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

int	handle_input_redirection(const char **input, t_command *cmd)
{
	char	buffer[1024];
	int		buf_index;

	buf_index = 0;
	(*input)++;
	if (**input == '<')
	{
		cmd->append_infd = 1;
		(*input)++;
		while (**input == ' ')
			(*input)++;
		parse_argument(input, buffer, &buf_index, cmd);
		cmd->append_infile = strdup(buffer);
	}
	else
	{
		cmd->input_fd = 1;
		while (**input == ' ')
			(*input)++;
		parse_argument(input, buffer, &buf_index, cmd);
		if (ft_strchr(buffer, '<'))
			{
				error_message("<<", cmd);
				return 1;
			}
		//cmd->input_file[cmd->input_fd] = strdup(buffer);
		//cmd->input_file[cmd->input_fd] = ft_strdup(buffer);
		cmd->input_file = ft_strdup(buffer);
		//cmd->input_fd++;
	}
	return 0;
}

int	handle_output_redirection(const char **input, t_command *cmd)
{
	char	buffer[1024];
	int		buf_index;

	buf_index = 0;
	(*input)++;
	if (**input == '>')
	{
		cmd->append_outfd = 1;
		(*input)++;
		while (**input == ' ')
			(*input)++;
		parse_argument(input, buffer, &buf_index, cmd);
		cmd->append_outfile = strdup(buffer);
	}
	else
	{
		cmd->output_fd = 1;
		while (**input == ' ')
			(*input)++;
		parse_argument(input, buffer, &buf_index, cmd);
		if (ft_strchr(buffer, '>'))
			{
				error_message(">>", cmd);
				return 1;
			}
		cmd->output_file = strdup(buffer);
	}
	return 0;
}

int	parse_redirection(const char **input, t_command *cmd)
{
	int result;
	if (**input == '<')
		result = handle_input_redirection(input, cmd);
	else if (**input == '>')
		result = handle_output_redirection(input, cmd);
	return result;
}
