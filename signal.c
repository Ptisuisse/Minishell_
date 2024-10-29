
#include "minishell.h"

int g_received_signal = 0;

void	handle_signal(int sig)
{
	g_received_signal = sig;

	if (sig == SIGINT || sig == SIGQUIT)
	{
	//	rl_replace_line("", 0);
		rl_on_new_line();
		write(1, "\n", 1);
		rl_redisplay();
	}
	if (sig == SIGSEGV)
	{
		//ft_printf("exit\n");
		exit(1);
	}
}

void	setup_signal_handling()
{
	g_received_signal = 0;
	signal(SIGINT, handle_signal);
	signal(SIGQUIT, SIG_IGN);
	signal(SIGSEGV, handle_signal);
}

int	handle_received_signal(int *save_exit_code)
{
	if (g_received_signal == SIGINT)
	{
		*save_exit_code = 130;
		g_received_signal = 0;
		return (1);
	}
	if (g_received_signal == SIGSEGV)
	{
		*save_exit_code = 0;
		g_received_signal = 0;
		return (1);
	}
	return (0);
}

// void	handle_signal(int sig, t_command *command_list)
// {
// 	if (sig == SIGINT)
// 	{
// 		command_list->exit_code = 130;
// 		rl_clear_history();
// 		write(1, "\nMiniBash > ", 13);
// 	}
// 	else if (sig == SIGSEGV)
// 		exit(1);
// 	return ;
// }

// int	signal_handle(t_command *command_list)
// {
// 	struct sigaction	sa;

// 	sa_handler = handle_signal(0, command_list);
// 	sa_flags = 0;
// 	sigemptyset(&sa.sa_mask);
// 	sigaction(SIGINT, &sa, NULL);
// 	sigaction(SIGSEGV, &sa, NULL);
// 	sa_handler = SIG_IGN;
// 	sigaction(SIGQUIT, &sa, NULL);
// 	return (0);
// }