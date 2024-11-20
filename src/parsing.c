/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lisambet <lisambet@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/17 13:05:38 by lvan-slu          #+#    #+#             */
/*   Updated: 2024/11/20 13:22:16 by lisambet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	parse_argument(const char **input, char *buffer, int *buf_index,
		t_command **cmd)
{
	skip_spaces(input);
	while (**input)
	{
		if (**input == '"' || **input == '\'')
		{
			handle_quotes(input, buffer, buf_index, cmd);
		}
		else if (**input == '$')
			handle_dollar_sign(input, buffer, buf_index, cmd);
		else if ((**input == '\\'))
		{
			(*input)++;
			buffer[(*buf_index)++] = *(*input)++;
		}
		else if ((**input == ' ') || (**input == '|'))
			break ;
		else
			buffer[(*buf_index)++] = *(*input)++;
	}
	buffer[*buf_index] = '\0';
}

void	handle_quotes(const char **input, char *buffer, int *buf_index,
		t_command **command_list)
{
	char	quote_type;

	quote_type = **input;
	(*input)++;
	while (**input && **input != quote_type)
	{
		if (quote_type == '"' && **input == '$' && ft_isalnum((*input)[1]))
			handle_dollar_sign(input, buffer, buf_index, command_list);
		else
			buffer[(*buf_index)++] = *(*input)++;
	}
	if (*buf_index == 0)
	{
		buffer[(*buf_index)++] = ' ';
		buffer[(*buf_index)++] = '\0';
	}
	if (**input == quote_type)
		(*input)++;
}

int	parse_arguments(const char **input, t_command **cmd, int *arg_index)
{
	char	buffer[1024];
	int		buf_index;

	buf_index = 0;
	parse_argument(input, buffer, &buf_index, cmd);
	if (buffer[0] != 0 || buf_index != 0)
		(*cmd)->args[(*arg_index)++] = ft_strdup(buffer);
	return (buf_index);
}

int	check_initial_conditions(const char *input, t_command **command_list,
		int exit_code, t_env **env_list)
{
	t_command	*new_node;

	if (open_quote((char *)input))
		return (-1);
	if (*input == '|')
	{
		new_node = init_command(exit_code, env_list);
		if (!new_node)
			return (1);
		append_command_node(command_list, new_node);
		error_message("|", command_list);
		return (1);
	}
	if (!(*input))
	{
		new_node = init_command(exit_code, env_list);
		if (!new_node)
			return (1);
		append_command_node(command_list, new_node);
		return (0);
	}
	return (-1);
}

void	ft_prompt(int save_exit_code, t_command *command_list, t_env **env_list,
		char *input)
{
	while (1)
	{
		setup_signal_handling();
		handle_received_signal(&save_exit_code);
		input = handle_input(input, save_exit_code, env_list, &command_list);
		if (process_input(&command_list, input, &save_exit_code, env_list))
		{
			if (command_list->error_file > 0 && command_list->next == NULL)
				check_error_file(command_list);
			else if (just_a_path(command_list))
			{
				check_heredoc(command_list);
				select_type(command_list, env_list);
			}
		}
		save_exit_code = last_exitcode(command_list);
		cleanup(&command_list, input);
		close_fd();
	}
}
