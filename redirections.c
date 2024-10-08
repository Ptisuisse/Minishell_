
#include "minishell.h"

void	handle_input_redirection(const char **input, t_command *cmd)
{
	char	buffer[1024];
	int		buf_index;

	buf_index = 0;
	(*input)++;
	cmd->input_fd = 1;
	while (**input == ' ')
		(*input)++;
	parse_argument(input, buffer, &buf_index);
	cmd->input_file = strdup(buffer);
}

void	handle_output_redirection(const char **input, t_command *cmd)
{
	char	buffer[1024];
	int		buf_index;

	buf_index = 0;
	(*input)++;
	cmd->output_fd = 1;
	if (**input == '>')
	{
		(*input)++;
		while (**input == ' ')
			(*input)++;
		parse_argument(input, buffer, &buf_index);
		cmd->append_file = strdup(buffer);
	}
	else
	{
		while (**input == ' ')
			(*input)++;
		parse_argument(input, buffer, &buf_index);
		cmd->output_file = strdup(buffer);
	}
}

void	parse_redirection(const char **input, t_command *cmd)
{
	if (**input == '<')
		handle_input_redirection(input, cmd);
	else if (**input == '>')
		handle_output_redirection(input, cmd);
}
