/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lvan-slu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/17 13:05:34 by lvan-slu          #+#    #+#             */
/*   Updated: 2024/10/17 13:05:35 by lvan-slu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H
# include "libft/libft.h"
# include <unistd.h>
# include <fcntl.h>
# include <stdio.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <stdbool.h>
# include <stdlib.h>
# include <string.h>
# include <sys/stat.h>
# include <sys/types.h>
# include <sys/wait.h>

# define MAX_TOKENS 100
# define READ_END 0
# define WRITE_END 1

//extern int g_exit_code;

typedef struct s_command
{
	int					status;
	int					exit_code;
	char				*command;
	char				*args[MAX_TOKENS];
	char				*input_file;
	int					input_fd;
	char				*output_file;
	int					output_fd;
	char				*append_infile;
	char				*append_outfile;
	int					append_infd;
	int					append_outfd;
	int					pipe[2];
	struct s_command	*next;
	struct s_command	*prev;
	pid_t				pid;
}						t_command;

typedef struct s_env
{
	char				*name;
	char				*value;
	char **env;
	struct s_env		*next;
}						t_env;

typedef struct s_data
{
	char				**env;
	t_env				*env_list;
}						t_data;

void free_command_list(t_command *command_list);
/*APPEND_FILE_C*/
void	append_file(t_command *command, t_env **env_list);

/*HEREDOC_C*/
void	check_heredoc(t_command *command);
void	write_to_heredoc(int pipe_fd_read);
void	heredoc_parent(t_command *command, int *pipe_fd, int stdin_backup);
void	read_heredoc(int pipe_fd_write, const char *end_of_input);
void	heredoc_child(t_command *command, int *pipe_fd);
void	heredoc(t_command *command);

/*PARSING_C*/

void	parse_argument(const char **input, char *buffer, int *buf_index, t_command *cmd);

void	handle_quotes(const char **input, char *buffer, int *buf_index, t_command *command_list);

int parse_arguments(const char **input, t_command *cmd, int *arg_index);

int parse_command(const char **input, t_command *cmd);

int	parse_command_line(const char *input, t_command **command_list, int exit_code);

/*PARSING_UTILS_C*/

t_command	*init_command(int exit_code);

t_command	*ft_lstlst(t_command *lst);

void	append_command_node(t_command **lst, t_command *new);

void	quoting_choice(bool *double_q, bool *sing_q, int *index, char c);

int	open_quote(const char *line);

/*DOLLAR_C*/

int	check_builtins(t_command *command, t_env **env_list);

void	process_dollar(const char **input, char *buffer, int *buf_index, t_command *command_list);

void	handle_dollar_sign(const char **input, char *buffer, int *buf_index, t_command *command_list);

int	handle_dollar(const char *input, int *i, char *result, int *result_index);

char	*search_dollar(const char *input, t_command *command_list);

/*ENV_VAR_C*/

void	skip_spaces(const char **input);

char	*get_env_value(const char *input, int *i);

/*REDIRECTIONS_C*/

void	handle_input_redirection(const char **input, t_command *cmd);

void	handle_output_redirection(const char **input, t_command *cmd);

void	parse_redirection(const char **input, t_command *cmd);

/*REDIRECTIONS_MANAGEMENT_C*/

void	redirect_input(t_command *command, t_env **env_list);

void	redirect_output(t_command *command, t_env **env_list);

void	redirect_management(t_command *command, t_env **env_list);

/*COMMAND_MANAGEMENT_C*/

void	handle_parent_process(t_command *commands);

void	handle_child_process(t_command *commands);

void	setup_pipes(t_command *commands);

void	commands_manager(t_command *commands, t_env **env_list);

void	start_builtins(t_command *command, t_env **env_list);

/*EXECUTION_H*/

int	check_path(char *pathname);

int	exec_command(t_command *command);

int	choose_command(t_command *command, t_env **env_list);

void	ft_process_wait(t_command *commands);

/*HEREDOC_C*/

void	check_heredoc(t_command *command);

/*BUILTINS_CD_PWD_C*/

void	update_env(t_env *env_list, char *name, char *value);

void	cd_cmd(t_command *command, t_env *env_list);

void	pwd_cmd(t_command *command);

/*BUILTINS_CLEAR_EXIT_C*/

void	clear_cmd(void);

int	exit_cmd(t_command *command);

/*BUILTINS_ECHO_C*/

void	echo_cmd(t_command *command);

int echo_print(char **args, int i);

/*BUILTINS_ENV_C*/

void	create_env_list(char **envp, t_env **env_list);

void	env_cmd(t_env *env_list);

void	unset_cmd(t_command *command, t_env *env_list);

/*BUILTINS_C*/

t_env	*export_args(char *arg, t_env *env_list);

t_env	*export_cmd(t_env *env_list, t_command *command);

/*BUILTINS_EXPORT_UTILS_C*/
int	print_error(t_command *command);

int	check_each_argument(t_command *command, int *equal);

int	ft_is_valid(t_command *command);

void	sorted_insert(t_env **head_ref, t_env *new_node);

void	sort_env_list(t_env **head_ref);

/*UTILS_C*/

void	printf_list(t_env *env_list);

char	*ft_strcpy(char *dst, const char *src);

char	*ft_strcat(char *dest, const char *src);

int    ft_strcmp(const char *s1, const char *s2);

void	print_commands(t_command *command_list);

void	free_commands(t_command *command_list);

/*SIGNAL_C*/

void	handle_signal(int sig);

int	signal_handle(void);

/*ERROR_C*/
void error_message(const char *token, t_command *cmd);
char	*replace_by_exit_code(char *result, int *result_index, t_command *command);

#endif
