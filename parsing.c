#include "minishell.h"

void	parse_argument(const char **input, char *buffer, int *buf_index)
{
	char	quote_type;

	skip_spaces(input);
	while (**input)
	{
		if (**input == '"' || **input == '\'')
		{
		printf("CCCCCC %c\n %s\n %c\n", **input, buffer, quote_type);
			quote_type = **input;
			handle_quotes(input, buffer, buf_index, quote_type);
			printf("DDDDDDD %c\n %s\n %c\n", **input, buffer, quote_type);
		}
		else if (**input == '$')
			handle_dollar_sign(input, buffer, buf_index);
		else if ((**input == ' ') || (**input == '|'))
			{
				printf("BBBBB %c\n %s\n %c\n", **input, buffer, quote_type);
			break ;}
		// else if (**input == '=')
		// 	{
		// 	while (**input != '\0')
		// 		input++;
		// 		//printf("BBBBB %c\n %s\n %c\n", **input, buffer, quote_type);
		// 	//break ;
		// 	}
		else
			{
			printf("AAAAAA %c\n %s\n %c\n", **input, buffer, quote_type);
			buffer[(*buf_index)++] = *(*input)++;}
	}
	buffer[*buf_index] = '\0';
}
void	handle_quotes(const char **input, char *buffer, int *buf_index, char quote_type)
{
	(*input)++;
	while (**input && **input != quote_type)
	{
		if (quote_type == '"' && **input == '$' && ft_isalpha((*input)[1]))
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

int parse_command(const char **input, t_command *cmd)
{
    int arg_index = 0;

    while (**input && **input != '|') {
        if (**input == ' ')
            (*input)++;
        else if (**input == '<' || **input == '>')
            parse_redirection(input, cmd);
        else
            parse_arguments(input, cmd, &arg_index);
    }
    cmd->args[arg_index] = NULL;

    if (**input == '|') {
        (*input)++;
        if (**input != ' ' && !ft_isalpha(**input))
            return (1);
    }
    return (0);
}

int	parse_command_line(const char *input, t_command **command_list)
{
	t_command	*new_node;

	if (open_quote((char *)input))
		return (1);
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
