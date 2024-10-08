#include "minishell.h"

void	parse_argument(const char **input, char *buffer, int *buf_index)
{
	char	quote_type;
	int		in_quotes;

	in_quotes = 0;
	skip_spaces(input);
	while (**input)
	{
		if (**input == '"' || **input == '\'')
		{
			in_quotes = 1; // = handle_quotes(input, buffer, buf_index, quote_type);
			quote_type = **input; 
			//printf("quotes %c\n", **input);
			handle_quotes(input, buffer, buf_index, quote_type);
			if (**input == quote_type)
				(*input)++;
		}
		else if (**input == '$')
			{
				handle_dollar_sign(input, buffer, buf_index);
				//printf("dollar %c\n", **input);
				}
		else if ((**input == ' ' && !in_quotes) || (**input == '|'))
			break;  
		else
			buffer[(*buf_index)++] = *(*input)++;
		in_quotes = 0; 
	}
	buffer[*buf_index] = '\0';  
}

void	handle_quotes(const char **input, char *buffer, int *buf_index,
	char quote_type)
{
	//int		in_quotes;
	if (quote_type == '"')
		handle_double_quotes(input, buffer, buf_index);
	else
		handle_single_quotes(input, buffer, buf_index);
	//ret
}

int parse_arguments(const char **input, t_command *cmd, int *arg_index) {
    char buffer[1024];
    int buf_index = 0;

    buf_index = 0;
    parse_argument(input, buffer, &buf_index);
    cmd->args[(*arg_index)++] = strdup(buffer);
    return buf_index;
}

int parse_command(const char **input, t_command *cmd) {
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
	while (*input) //<<--- leak : Invalid read of size 1
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
