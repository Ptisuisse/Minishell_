#include "minishell.h"
void parse_argument(const char **input, char *buffer, int *buf_index) {
    char quote_type;
    int in_quotes = 0;

    skip_spaces(input);
    while (**input) {
        if (**input == '"' || **input == '\'') {
            quote_type = **input; 
            in_quotes = handle_quotes(input, buffer, buf_index, quote_type);
            // Move past the closing quote
            if (**input == quote_type) {
                (*input)++;
            }
        } else if (**input == '$' && !in_quotes) {
            handle_dollar_sign(input, buffer, buf_index);
        } else if ((**input == ' ' && !in_quotes) || (**input == '|')) {
            break;  
        } else {
            buffer[(*buf_index)++] = *(*input)++;
        }
    }
    buffer[*buf_index] = '\0';  
}
int handle_quotes(const char **input, char *buffer, int *buf_index, char quote_type) {
    if (quote_type == '"') {
        return handle_double_quotes(input, buffer, buf_index);
    } else {
        return handle_single_quotes(input, buffer, buf_index);
    }
}

// void	parse_argument(const char **input, char *buffer, int *buf_index)
// {
// 	char	quote_type;
// 	int		in_quotes;

// 	in_quotes = 0;
// 	skip_spaces(input);
// 	while (**input)
// 	{
// 		if (**input == '"' || **input == '\'')
// 		{
// 			quote_type = **input; 
// 			in_quotes = handle_quotes(input, buffer, buf_index, quote_type);
// 			printf("quotes %s\n %c\n %d\n", buffer, **input, in_quotes);
// 			if (**input == quote_type)
// 				(*input)++;
// 		}
// 		else if (**input == '$'&& !in_quotes)
// 			{
// 				handle_dollar_sign(input, buffer, buf_index);
// 				//printf("dollar %c\n", **input);
// 				}
// 		else if ((**input == ' ' && !in_quotes) || (**input == '|'))
// 			break;  
// 		else 
// 			buffer[(*buf_index)++] = *(*input)++;
// 		//rintf("quotes 2 %s\n", *input);
// 		//in_quotes = 0; 
// 	}
// 	buffer[*buf_index] = '\0';  
// }

// int	handle_quotes(const char **input, char *buffer, int *buf_index,
// 	char quote_type)
// {
// 	int		in_quotes;

// 	in_quotes = 1;
// 	if (quote_type == '"')
// 		in_quotes = handle_double_quotes(input, buffer, buf_index);
// 	else
// 		handle_single_quotes(input, buffer, buf_index);
// 	return in_quotes;
// }

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
