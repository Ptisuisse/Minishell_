/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_management.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lvan-slu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/17 13:04:55 by lvan-slu          #+#    #+#             */
/*   Updated: 2024/10/17 13:04:56 by lvan-slu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	handle_parent_process(t_command *commands)
{
	if (commands->next)
		close(commands->pipe[WRITE_END]);
	if (commands->prev)
	{
		close(commands->prev->pipe[WRITE_END]);
		close(commands->prev->pipe[READ_END]);
	}
}

void	handle_child_process(t_command *commands)
{
	if (commands->prev)
	{
		close(commands->prev->pipe[WRITE_END]);
		dup2(commands->prev->pipe[READ_END], STDIN_FILENO);
		close(commands->prev->pipe[READ_END]);
	}
	if (commands->next)
	{
		close(commands->pipe[READ_END]);
		dup2(commands->pipe[WRITE_END], STDOUT_FILENO);
		close(commands->pipe[WRITE_END]);
	}
}

void	setup_pipes(t_command *commands)
{
	if (commands->next)
		pipe(commands->pipe);
	commands->pid = fork();
}

void	gestion_redirection(t_command *commands)
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
		//ft_printf("bash: %s: No such file or directory\n", commands->input_file);
		commands->exit_code = 1;
		close(fd);
		return ;
	}
	if (access(filename, W_OK) == -1)
    {
        //ft_printf("%s: Permission denied\n", filename);
        commands->exit_code = 1;
        return ;
    }
	close (fd);
	return ;
}

void	start_builtins(t_command *command, t_env **env_list)
{
	if (!(ft_strcmp(command->args[0], "exit")))
		exit_cmd(command);
	else if (!(ft_strcmp(command->args[0], "echo")))
		echo_cmd(command);
	else if (!(ft_strcmp(command->args[0], "cd")))
		cd_cmd(command, *env_list);
	else if (!(ft_strcmp(command->args[0], "pwd")))
		pwd_cmd(command);
	else if (!(ft_strcmp(command->args[0], "env")))
		env_cmd(*env_list);
	else if (!(ft_strcmp(command->args[0], "export")))
		*env_list = export_cmd(*env_list, command);
	else if (!(ft_strcmp(command->args[0], "unset")))
		unset_cmd(command, *env_list);
}
