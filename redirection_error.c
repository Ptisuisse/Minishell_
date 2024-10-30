#include "minishell.h"

int	parsing_error_inputfile(t_command *commands)
{
	char	*filename;
	int		fd;

	if (commands->input_fd)
		filename = commands->input_file;
	else
		filename = commands->append_infile;
	fd = open(filename, O_RDONLY);
	if (fd < 0)
	{
		close(fd);
		return (0);
	}
	if (access(filename, W_OK) == -1)
	{
		close(fd);
        return (0);
    }
	close (fd);
	return (1);
}

int	parsing_error_outputfile(t_command *commands)
{
	char	*filename;
	int fd;

	if (commands->output_fd)
		filename = commands->output_file;
	else
		filename = commands->append_outfile;
	fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd < 0)
	{
		close(fd);
		return (0);
	}
	if (access(filename, W_OK) == -1)
    {
        close(fd);
        return (0);
    }
	close (fd);
	return (1);
}