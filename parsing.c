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

void	parse_argument(const char **input, char *buffer, int *buf_index)
{
	char	quote_type;

	skip_spaces(input);
	while (**input)
	{
		if (**input == '"' || **input == '\'')
		{
			quote_type = **input;
			handle_quotes(input, buffer, buf_index, quote_type);
		}
		else if (**input == '$')
				handle_dollar_sign(input, buffer, buf_index);
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
void	handle_quotes(const char **input, char *buffer, int *buf_index, char quote_type)
{
	(*input)++;
	while (**input && **input != quote_type)
	{
		if (quote_type == '"' && **input == '$' && ft_isalnum((*input)[1]))
			handle_dollar_sign(input, buffer, buf_index);
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
    parse_argument(input, buffer, &buf_index);
    cmd->args[(*arg_index)++] = strdup(buffer);
    return buf_index;
}

int handle_redirection_and_arguments(const char **input, t_command *cmd, int *arg_index)
{
    char *token;

    if (**input == '<' || **input == '>')
    {
        if (*(*input + 1) == '>' || *(*input + 1) == '<')
        {
            if (*(*input + 2) == '<')
                token = "<";
            else if (*(*input + 2) == '>')
                token = ">";
            else
                token = "newline";
            error_message(token);
            return (1);
        }
        else if (*(*input + 1) && *(*input + 1) != ' ' && !ft_isalpha(*(*input + 1)))
        {
            token = "|";
            error_message(token);
            return (1);
        }
        else if (*(*input + 1) == '\0' || *(*input + 1) == '|')
        {
            token = "newline";
            error_message(token);
            return (1);
        }
        parse_redirection(input, cmd);
        (*arg_index)++;
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
            error_message("|");
            return (1);
        }
    }
    return (0);
}


// int parse_command(const char **input, t_command *cmd)
// {
//     int arg_index = 0;
//     char *token = NULL;

//     while (**input && **input != '|') {
//         if (**input == ' ')
//             (*input)++;
//         else if ((**input == '<' || **input == '>'))
// 		{
// 			if (*(*input+1) == '>' || *(*input+1) == '<')
// 				{
// 				if (*(*input+2) == '<')
// 					token = "<";
// 				else if (*(*input+2) == '>') 
// 					token = ">";
//                 else
// 					token = "newline";
//                 error_message(token);
//                 return 1;
//             	}
// 			else if (*(*input+1) && *(*input+1) != ' ' && !ft_isalpha(*(*input+1))) 
// 				{
// 				token = "|";
//                 error_message(token);
//                 return 1;
// 				}
// 			else if (*(*input + 1) == '\0' || *(*input + 1) == '|') {
//                 token = "newline";
//                 error_message(token);
//                 return 1;
//             }
//             parse_redirection(input, cmd);
//             arg_index++;
//         }
//         else {
//             parse_arguments(input, cmd, &arg_index);
//         }
//     }
//     cmd->args[arg_index] = NULL;
//     if (**input == '|') {
//         (*input)++;
//         if (**input != ' ' && !ft_isalpha(**input)) {
//             token = "|";
//             error_message(token);
//             return 1;
//         }
//     }
//     return 0;
// }

int	parse_command_line(const char *input, t_command **command_list)
{
	t_command	*new_node;

	if (open_quote((char *)input))
		return (1);
	if (*input == '|')
		{
			error_message("|");
            return 1;
		}
	while (*input)
	{
		new_node = init_command();
		if (!new_node)
			return (1);
		if (!parse_command(&input, new_node))
			append_command_node(command_list, new_node);
		else
			return (1);
	}
	return (0);
}
