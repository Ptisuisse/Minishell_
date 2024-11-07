/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_commands.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lisambet <lisambet@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/05 14:30:42 by lisambet          #+#    #+#             */
/*   Updated: 2024/11/07 13:29:53 by lisambet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	check_and_init_command(const char *input, t_command **command_list,
		int exit_code)
{
	int			check;
	t_command	*new_node;

	check = check_initial_conditions(input, command_list, exit_code);
	if (check != -1)
		return (check);
	if (*input == '|')
	{
		new_node = init_command(exit_code);
		if (!new_node)
			return (1);
		append_command_node(command_list, new_node);
		error_message("|", *command_list);
		return (1);
	}
	return (0);
}

int	parse_and_append_command(const char **input, t_command *new_node,
		t_command **command_list)
{
	if (parse_command(input, new_node))
	{
		append_command_node(command_list, new_node);
		return (1);
	}
	if (new_node->args[0] != NULL)
		append_command_node(command_list, new_node);
	else
	{
		while (**input == '|' || **input)
			(*input)++;
		if (**input == '|')
			(*input)++;
		parse_command(input, new_node);
		append_command_node(command_list, new_node);
		return (1);
	}
	return (0);
}

int	process_input_commands(const char *input, t_command **command_list,
		int exit_code)
{
	t_command	*new_node;

	while (*input)
	{
		new_node = init_command(exit_code);
		if (!new_node)
			return (1);
		if (parse_and_append_command(&input, new_node, command_list))
			return (1);
	}
	return (0);
}

int	parse_command_line(const char *input, t_command **command_list,
		int exit_code)
{
	if (check_and_init_command(input, command_list, exit_code))
		return (1);
	return (process_input_commands(input, command_list, exit_code));
}

int	parse_command(const char **input, t_command *cmd)
{
	int	arg_index;

	arg_index = 0;
	while (**input && **input != '|')
	{
		if (**input == ' ')
			(*input)++;
		else
		{
			if (handle_redirection_and_arguments(input, cmd, &arg_index))
				return (1);
		}
	}
	// if (cmd->file > arg_index)
	// 	arg_index = cmd->file + 1;
	cmd->args[arg_index] = NULL;
	if (**input == '|')
	{
		(*input)++;
		if ((**input != ' ' && !ft_isalpha(**input)) || *(*input + 1) == '|' || **input == '|')
		{
			error_message("|", cmd);
			return (1);
		}
	}
	return (0);
}
