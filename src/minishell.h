/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lisambet <lisambet@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/17 13:05:34 by lvan-slu          #+#    #+#             */
/*   Updated: 2024/11/19 17:34:27 by lisambet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H
# include "libft/libft.h"
# include <curses.h>
# include <fcntl.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <stdbool.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/stat.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <unistd.h>

# define MAX_TOKENS 100
# define READ_END 0
# define WRITE_END 1

extern int				g_received_signal;

typedef struct s_env
{
	char				*name;
	char				*value;
	char				**env;
	struct s_env		*next;
}						t_env;

typedef struct s_command
{
	int					save_in;
	int					save_out;
	int					file;
	int					output;
	int					error_file;
	int					status;
	int					exit_code;
	char				*error_message;
	char				*args[MAX_TOKENS];
	char				*input_file;
	char				*output_file;
	char				*append_file;
	char				*heredoc_file;
	int					pipe[2];
	struct s_command	*next;
	struct s_command	*prev;
	pid_t				pid;
	t_env				*env;
}						t_command;

typedef struct s_data
{
	char				**env;
	t_env				*env_list;
	t_command			*command;
}						t_data;

void					ft_prompt(int save_exit_code, t_command *command_list,
							t_env **env_list, char *input);
int						validate_path(const char *path, t_command *command);
int						process_command_path(const char *path,
							t_command *command);
void					cleanup(t_command **command_list, char *input);
void					process_commands(t_command **command_list, char *input,
							int *save_exit_code, t_env **env_list);
char					*handle_input(char *input, int save_exit_code,
							t_env **env_list, t_command **command_list);
void					setup_environment(t_env **env_list, char **envp);
void					handle_signal_parent(int g_received_signal);
void					handle_signal_child(int g_received_signal);
void					process_child_pipe(t_command *commands,
							t_env **env_list, int *prev_pipe_fd);
void					setup_pipes(t_command *commands);
void					wait_for_commands(t_command *commands);
void					append_node(t_env *env_list, char *value);
void					print_variables(t_env **head);
int						update_existing_var(t_env *tmp, char *name, char *value,
							int append);
int						ft_isspace(char c);
int						is_numeric(const char *str);
void					handle_exit_error(char *arg);
void					handle_exit_code(char *arg, int *exit_code);
int						parsing_error_appendfile(t_command *commands);
int						check_file(const char *filename, t_command *commands);
int						multiple_redirection_output(t_command *command,
							t_env **env_list);
void					multiple_redirection(t_command *command,
							t_env **env_list);
int						multiple_redirection_input(t_command *command,
							t_env **env_list);
void					multiple_redirection_exec(t_command *command,
							t_env **env_list);
void					advance_to_end_or_pipe(const char **input);
void					free_split(char **split);
char					**create_envp(t_env *env_list);
void					check_error_file(t_command *cmd);
int						just_a_path(t_command *command);
char					*find_path(t_env **env_list, t_command *command,
							char *cmd);
int						choose_command_pipe(t_command *command,
							t_env **env_list);
void					select_type(t_command *command, t_env **list);
void					setup_signal_handling(void);
/*APPEND_FILE_C*/
void					append_file(t_command *command, t_env **env_list);

/*HEREDOC_C*/
void					check_heredoc(t_command *command);
void					write_to_heredoc(int pipe_fd_read);
void					heredoc_parent(t_command *command, int *pipe_fd);
void					read_heredoc(int pipe_fd_write,
							const char *end_of_input);
void					heredoc_child(t_command *command, int *pipe_fd);
void					heredoc(t_command *command);

/*PARSING_C*/
char					*handle_special_cases(const char *input, int *i);
char					*handle_remaining_chars(const char *input, int *i,
							int *result_index);
void					skip_quotes(const char *input, int *i);
char					*extract_env_key(const char *input);
char					*find_env_value(const char *env_key, t_env *env);
char					*process_character(const char *input, int *i,
							int *result_index, t_command **command_list);
char					*concatenate_results(char *result, char *temp);
char					*process_dollar_case(const char *input, int *i,
							int *result_index, t_command **command_list);
void					parse_argument(const char **input, char *buffer,
							int *buf_index, t_command **cmd);

void					handle_quotes(const char **input, char *buffer,
							int *buf_index, t_command **command_list);

int						parse_arguments(const char **input, t_command **cmd,
							int *arg_index);

int						parse_command(const char **input, t_command **cmd);

int						parse_command_line(const char *input,
							t_command **command_list, int exit_code,
							t_env **env_list);
int						check_and_init_command(const char *input,
							t_command **command_list, int exit_code,
							t_env **env_list);
int						parse_and_append_command(const char **input,
							t_command *new_node, t_command **command_list);
int						process_input_commands(const char *input,
							t_command **command_list, int exit_code,
							t_env **env_list);
int						check_initial_conditions(const char *input,
							t_command **command_list, int exit_code,
							t_env **env_list);

/*PARSING_UTILS_C*/

int						process_input(t_command **command_list, char *input,
							int *save_exit_code, t_env **env_list);

t_command				*init_command(int exit_code, t_env **env_list);

t_command				*ft_lstlst(t_command *lst);

void					append_command_node(t_command **lst, t_command *new);

void					quoting_choice(bool *double_q, bool *sing_q, int *index,
							char c);

int						open_quote(const char *line);

/*DOLLAR_C*/

int						check_builtins(t_command *command, t_env **env_list);

void					process_dollar(const char **input, char *buffer,
							int *buf_index, t_command **command_list);

void					handle_dollar_sign(const char **input, char *buffer,
							int *buf_index, t_command **command_list);

char					*handle_dollar(const char *input, int *i,
							int *result_index, t_command **command_list);

char					*search_dollar(const char *input,
							t_command **command_list);

/*ENV_VAR_C*/

void					skip_spaces(const char **input);

/*REDIRECTIONS_C*/

int						detect_invalid_double_redirection(const char **input,
							char **token);

int						handle_redirection_error(const char *token,
							t_command **cmd);

int						check_double_redirection(const char **input,
							t_command **cmd);

int						handle_redirection(const char **input, t_command **cmd);

int						handle_redirection_and_arguments(const char **input,
							t_command **cmd, int *arg_index);

int						handle_input_redirection(const char **input,
							t_command **cmd);

int						handle_output_redirection(const char **input,
							t_command **cmd);

int						parse_redirection(const char **input, t_command **cmd);

/*REDIRECTIONS_MANAGEMENT_C*/

int						redirect_input(t_command *command, t_env **env_list);

int						redirect_output(t_command *command, t_env **env_list);

void					redirect_management(t_command *command,
							t_env **env_list);

/*COMMAND_MANAGEMENT_C*/

void					handle_parent_process(t_command *commands);

void					handle_child_process(t_command *commands);

void					setup_pipes(t_command *commands);

void					commands_pipe_manager(t_command *commands,
							t_env **env_list);

void					start_builtins(t_command *command, t_env **env_list);

/*EXECUTION_H*/

int						check_path(char *pathname);

int						exec_command(t_command *command, t_env **env_list);

int						choose_command(t_command *command, t_env **env_list);

void					ft_process_wait(t_command *commands);

/*HEREDOC_C*/
int						handle_heredoc(const char **input, t_command *cmd,
							char *buffer);
void					handle_heredoc_signal(int sig);
void					setup_heredoc_signal_handling(void);
void					check_heredoc(t_command *command);

/*BUILTINS_CD_PWD_C*/

void					update_env(t_env *env_list, char *name, char *value);

void					cd_cmd(t_command *command, t_env *env_list);

void					pwd_cmd(t_command *command);

/*BUILTINS_CLEAR_EXIT_C*/

int						exit_cmd(t_command *command, t_env **env_list,
							int save_exit_code);

/*BUILTINS_ECHO_C*/

void					echo_cmd(t_command *command);

int						echo_print(char **args, int i);

/*BUILTINS_ENV_C*/

void					create_env_list(char **envp, t_env **env_list);

void					env_cmd(t_env *env_list);

void					unset_cmd(t_command *command, t_env **env_list);

/*BUILTINS_C*/

t_env					*export_args(char *arg, t_env *env_list);

t_env					*export_cmd(t_env *env_list, t_command *command);

/*BUILTINS_EXPORT_UTILS_C*/
int						print_error(t_command *command);

int						check_each_argument(t_command *command, int *equal,
							int j);

int						ft_is_valid(t_command *command);

void					sorted_insert(t_env **head_ref, t_env *new_node);

void					sort_env_list(t_env **head_ref);

/*UTILS_C*/

void					printf_list(t_env *env_list);

char					*ft_strcpy(char *dst, const char *src);

char					*ft_strcat(char *dest, const char *src);

int						ft_strcmp(const char *s1, const char *s2);

void					print_commands(t_command *command_list);

void					free_commands(t_command *command_list);

/*SIGNAL_C*/

int						handle_received_signal(int *save_exit_code);

void					handle_signal(int sig);

int						signal_handle(void);

void					setup_signal_handling(void);

/*ERROR_C*/
int						last_exitcode(t_command *command);
void					error_message(const char *token, t_command **cmd);
char					*replace_by_exit_code(char *result, int *result_index,
							t_command **command);

void					free_env_list(t_env **env_list);
int						choose_command_pipe(t_command *command,
							t_env **env_list);
void					exec_pipe_command(t_command *command, t_env **env_list);
void					put_into_args(t_command *commands);
int						parsing_error_inputfile(t_command *commands, char *buffer);
int						parsing_error_outputfile(t_command *commands,
							char *buffer);
void					select_type(t_command *command, t_env **list);
void					setup_signal_handling(void);
void					free_command_list(t_command **command_list);

#endif