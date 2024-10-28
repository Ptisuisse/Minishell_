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

int handle_redirection_and_arguments(const char **input, t_command *cmd, int *arg_index)
{
    char *token = NULL;

    if (**input == '<' || **input == '>')
    {
        if (*(*input + 1) == '>' || *(*input + 1) == '<')
        {
            if (*(*input + 2) == '<')
                token = "<";
            else if (*(*input + 2) == '>')
                token = ">";
            if (token)
            {
                token = "newline";
                error_message(token, cmd);
                return (1);
            }
        }
        else if (*(*input + 1) && *(*input + 1) != ' ' && !ft_isascii(*(*input + 1)))
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
        if (parse_redirection(input, cmd))
            return 1;
        //(*arg_index)++;
    }
    else
        parse_arguments(input, cmd, arg_index);
    return (0);
}

int parse_command(const char **input, t_command *cmd)
{
    int arg_index;

    arg_index = 0;
    // cmd->exit_code = 0;
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

int	parse_command_line(const char *input, t_command **command_list, int exit_code)
{
    t_command *new_node;

	if (open_quote((char *)input))
		return (1);
	if (*input == '|')
	{
		error_message("|", *command_list);
        return 1;
	}
    if (!(*input))
    {
        new_node = init_command(exit_code);
        if (!new_node)
			return (1);
        append_command_node(command_list, new_node);
        return (0);
    }
	while (*input)
	{
		new_node = init_command(exit_code);
		if (!new_node)
			return (1);
		parse_command(&input, new_node);
        //fonction pour checker les differents input (args, append, fd etc)
         if (new_node->args[0] != NULL)
		 	append_command_node(command_list, new_node);
         else
         {
             while ((*input == '|') || (*input))
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
