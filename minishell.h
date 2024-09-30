#ifndef MINISHELL_H
# define MINISHELL_H
# include "libft/libft.h"
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <stdbool.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <unistd.h>

# define MAX_TOKENS 100
# define READ_END 0
# define WRITE_END 1

typedef struct s_command
{
	char				*command;
	char				*args[MAX_TOKENS];
	char				*input_file;
	char				*output_file;
	char				*append_file;
	int					pipe[2];
	struct s_command	*next;
	struct s_command	*prev;
	pid_t					pid;
}						t_command;

// typedef struct s_command_node
//	{
//    t_command *cmd;
//    struct s_command_node *next;
//} t_command;

typedef struct s_data
{
	char				**env;
	// struct s_data	next;
}						t_data;

/*prompt*/

/*utils.*/


void	print_commands(t_command *command_list);
void					free_commands(t_command *command_list);
int						open_quote(const char *line);
t_command				*init_command(void);
void					append_command_node(t_command **head,
							t_command *new_node);
char					*ft_strcpy(char *dst, const char *src);
char					*ft_strcat(char *dst, const char *src);
int						ft_strcmp(const char *s1, const char *s2);
/*commands*/
void	start_builtins(char **command);
int	choose_command(t_command *command);
void	env_cmd(t_data data);
int	echo_cmd(char **args);
int	cd_cmd(char *path);
int	pwd_cmd(void);
/*pipe_management*/
void execute_commands(t_command *commands);
int	exec_command(char *pathname, char **args);
/*parsing*/


void	parse_argument(const char **input, char *buffer, int *buf_index);
void	parse_redirection(const char **input, t_command *cmd);
int check_for_pipe(const char **input);
char	*get_env_value(const char *input, int *i);
int	check_quotes(const char **input, char *quote_type);
void	handle_quotes(const char **input, char *buffer, int *buf_index,char quote_type);
void	skip_spaces(const char **input);
void	handle_dollar_sign(const char **input, char *buffer, int *buf_index);
void	handle_double_quotes(const char **input, char *buffer, int *buf_index);
void	handle_single_quotes(const char **input, char *buffer, int *buf_index);
char *search_dollar(const char *input);
//void					print_commands(t_command *command_list);
int						parse_command_line(const char *input,
							t_command **command_list);
int						parse_command(const char **input, t_command *cmd);

/*signal*/
void	handle_signal(int sig);
int	signal_handle(void);


#endif