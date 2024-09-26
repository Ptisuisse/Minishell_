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
	int					pid;
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
t_command *ft_lstlst(t_command *lst);
void					command_manager(t_command *command_list, t_data data);
/*utils.*/
void					free_commands(t_command *command_list);
int						open_quote(char *line);
t_command				*init_command(void);
void					append_command_node(t_command **head,
							t_command *new_node);
char					*ft_strcpy(char *dst, const char *src);
char					*ft_strcat(char *dst, const char *src);
int						ft_strcmp(const char *s1, const char *s2);
/*commands*/
int						pwd_cmd(void);
int						check_builtins(char **command, t_data data);
int						cd_cmd(char *path);
int						execute_command(char *pathname, char **args);
int						select_commands(t_command *commands, t_data data);
void					start_builtins(char **command, t_data data);
int						echo_cmd(char **args);
void					env_cmd(t_data data);
// void		create_fork(char *pathname, char **args);
/*parsing*/
void					print_commands(t_command *command_list);
int						parse_command_line(const char *input,
							t_command **command_list);
int						parse_command(const char **input, t_command *cmd);
/*signal*/
void					ft_pipe(t_command *command, t_data data);
void					handle_signal(int sig);
int						signal_handle(void);
void					ft_create_pipe(t_command *command, t_data data);
int						choose_command(t_command *command, t_data data);
int						execute_piped_commands(t_command *command);

#endif