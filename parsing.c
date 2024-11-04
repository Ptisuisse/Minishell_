/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lvan-slu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/17 13:05:38 by lvan-slu          #+#    #+#             */
/*   Updated: 2024/10/17 13:05:39 by lvan-slu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	parse_argument(const char **input, char *buffer, int *buf_index, t_command *cmd)
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

void	handle_quotes(const char **input, char *buffer, int *buf_index, t_command *command_list)
{
    char    quote_type;

    quote_type = **input;
	(*input)++;
	while (**input && **input != quote_type)
	{
		if (quote_type == '"' && **input == '$' && ft_isalnum((*input)[1]))
			handle_dollar_sign(input, buffer, buf_index, command_list);
		else
			buffer[(*buf_index)++] = *(*input)++;
	}
	if (**input == quote_type)
		(*input)++;
}

int parse_arguments(const char **input, t_command *cmd, int *arg_index)
{
    char buffer[1024];
    int buf_index = 0;

    buf_index = 0;
    parse_argument(input, buffer, &buf_index, cmd);
    cmd->args[(*arg_index)++] = strdup(buffer);
    return buf_index;
}

int check_double_redirection(const char **input, t_command *cmd)
{
    char *token = NULL;

    if (*(*input + 1) == '>' || *(*input + 1) == '<')
    {
        if (*(*input + 2) == '<' &&	*(*input + 3) != '\0')
        {
			token = "newline";
		    if (*(*input + 3) == '<')
				token = "<";
			if (*(*input + 4) == '<')
				token = "<<";
		}
		else if (*(*input + 2) == '>')
		{
			token = ">";
            if (*(*input + 3) == '>')
				token = ">>";
		}
		// else if (cmd->append_file == NULL)
		// 	token = "newline";
        if (token)
        {
            error_message(token, cmd);
            return (1);
        }
    }
    return (0);
}

int handle_redirection(const char **input, t_command *cmd)
{
    char *token = NULL;

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
    return parse_redirection(input, cmd);
}

int handle_redirection_and_arguments(const char **input, t_command *cmd, int *arg_index)
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

int parse_command(const char **input, t_command *cmd)
{
    int arg_index;

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
    cmd->args[arg_index] = NULL;
    if (**input == '|')
    {
        (*input)++;
        if (**input != ' ' && !ft_isalpha(**input))
        {
            error_message("|", cmd);
            return (1);
        }
    }
    return (0);
}

int	check_initial_conditions(const char *input, t_command **command_list, int exit_code)
{
    t_command *new_node;

	if (open_quote((char *)input))
		return (1);
	if (*input == '|')
	{
		new_node = init_command(exit_code);
		if (!new_node)
			return (1);
		append_command_node(command_list, new_node);
		error_message("|", *command_list);
		return (1);
	}
	if (!(*input))
	{
		new_node = init_command(exit_code);
		if (!new_node)
			return (1);
		append_command_node(command_list, new_node);
		return (0);
	}
	return (-1);
}
int	check_and_init_command(const char *input, t_command **command_list, int exit_code)
{
	int	check;
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

int	process_input_commands(const char *input, t_command **command_list, int exit_code)
{
	t_command	*new_node;

	while (*input)
	{
		new_node = init_command(exit_code);
		if (!new_node)
			return (1);
		if (parse_command(&input, new_node))
		{
			append_command_node(command_list, new_node);
			return 1;
		}
		if (new_node->args[0] != NULL)
			append_command_node(command_list, new_node);
		else
		{
			while (*input == '|' || *input)
				input++;
			if (*input == '|')
				input++;
			parse_command(&input, new_node);
			append_command_node(command_list, new_node);
			return (1);
		}
	}
	return (0);
}

int	parse_command_line(const char *input, t_command **command_list, int exit_code)
{
	if (check_and_init_command(input, command_list, exit_code))
		return (1);
	return (process_input_commands(input, command_list, exit_code));
}
