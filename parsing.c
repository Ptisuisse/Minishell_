#include "minishell.h"

int	check_builtins(char **command, t_data data)
{
	int	status;

	// printf("Je suis dans check_builtins\n");
	status = 0;
	if (ft_strcmp(command[0], "$"))
		status = 1;
	if (ft_strcmp(command[0], "exit"))
		status = 1;
	else if (ft_strcmp(command[0], "echo"))
		status = 1;
	else if (ft_strcmp(command[0], "cd"))
		status = 1;
	else if (ft_strcmp(command[0], "export"))
		status = 1;
	else if (ft_strcmp(command[0], "unset"))
		status = 1;
	else if (ft_strcmp(command[0], "env"))
		status = 1;
	else if (ft_strcmp(command[0], "pwd"))
		status = 1;
	if (status == 1)
		start_builtins(command, data);
	return (status);
}

void	parse_argument(const char **input, char *buffer, int *buf_index)
{
	char	quote_type;
	int		in_quotes;

	in_quotes = 0;
	while (**input == ' ')
		(*input)++;
	while (**input)
	{
		if (**input == '"' || **input == '\'')
		{
			quote_type = **input;
			(*input)++;
			in_quotes = 1;
			while (**input && **input != quote_type)
				buffer[(*buf_index)++] = *(*input)++;
			if (**input == quote_type)
				(*input)++;
		}
		else if (**input == ' ' && !in_quotes)
			break ;
		else
			buffer[(*buf_index)++] = *(*input)++;
		in_quotes = 0;
	}
	buffer[*buf_index] = '\0';
}

void	parse_redirection(const char **input, t_command *cmd)
{
	char	buffer[1024];
	int		buf_index;

	buf_index = 0;
	if (**input == '<')
	{
		(*input)++;
		while (**input == ' ')
			(*input)++;
		parse_argument(input, buffer, &buf_index);
		cmd->input_file = strdup(buffer);
	}
	else if (**input == '>')
	{
		(*input)++;
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
}

int	parse_command(const char **input, t_command *cmd)
{
	char	buffer[1024];
	int		buf_index;
	int		arg_index;

	buf_index = 0;
	arg_index = 0;
	while (**input && **input != '|')
	{
		buf_index = 0;
		if (**input == ' ')
			(*input)++;
		else if (**input == '<' || **input == '>')
			parse_redirection(input, cmd);
		else
		{
			parse_argument(input, buffer, &buf_index);
			cmd->args[arg_index++] = strdup(buffer);
		}
	}
	cmd->args[arg_index] = NULL;
	if (**input == '|')
	{
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
	// if ((ft_strchr(*input, '$') != NULL) || (ft_strchr(*input, '~') != NULL))
	//	*input = dollar(*input);
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

void	print_commands(t_command *command_list)
{
	int	i;
	int	j;

	i = 0;
	while (command_list)
	{
		printf("Command[%d]:\n", i);
		j = 0;
		while (command_list->args[j])
		{
			printf("  Arg[%d]: %s\n", j, command_list->args[j]);
			j++;
		}
		if (command_list->input_file)
			printf("  Input Redirection: %s\n", command_list->input_file);
		if (command_list->output_file)
			printf("  Output Redirection: %s\n", command_list->output_file);
		if (command_list->append_file)
			printf("  Append Redirection: %s\n", command_list->append_file);
		command_list = command_list->next;
		i++;
	}
}

//void	free_commands(t_command *command_list)
//{
//	int			j;
//	t_command	*temp;

//	while (command_list)
//	{
//		temp = command_list;
//		command_list = command_list->next;
//		j = 0;
//		while (temp->args[j])
//			free(temp->args[j++]);
//		free(temp->input_file);
//		free(temp->output_file);
//		free(temp->append_file);
//		free(temp);
//	}
//}

void	free_commands(t_command *command_list)
{
	t_command	*tmp;

	if (!command_list)
		return ;
	while (command_list)
	{
		tmp = command_list->next;
		free(command_list);
		command_list = tmp;
	}
	command_list = NULL;
	free(command_list);
}
