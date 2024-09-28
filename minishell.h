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
int	ft_strcmp(const char *s1, const char *s2);
char	*ft_strcat(char *dest, const char *src);
char	*ft_strcpy(char *dst, const char *src);
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
int	check_builtins(char **command);
void	parse_argument(const char **input, char *buffer, int *buf_index);
void	parse_redirection(const char **input, t_command *cmd);
int	parse_command(const char **input, t_command *cmd);
int	parse_command_line(const char *input, t_command **command_list);
void	print_commands(t_command *command_list);
void	free_commands(t_command *command_list);
/*parsingutils*/
t_command	*init_command(void);
t_command	*ft_lstlst(t_command *lst);
void	append_command_node(t_command **lst, t_command *new);
void	quoting_choice(bool *double_q, bool *sing_q, int *index, char c);
int	open_quote(char *line);
/*signal*/
void	handle_signal(int sig);
int	signal_handle(void);


#endif