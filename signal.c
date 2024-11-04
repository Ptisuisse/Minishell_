#include "minishell.h"

//int g_received_signal = 0;

void	handle_signal(int sig)
{
	g_received_signal = sig;

	if (sig == SIGINT || sig == SIGQUIT)
	{
		rl_replace_line("", 0);
		rl_on_new_line();
		write(1, "\n", 1);
		rl_redisplay();
	}
}
void	handle_heredoc_signal(int sig)
{
	if (sig == SIGINT)
	{
		g_received_signal = 1;  
		rl_done = 1;            
		rl_replace_line("", 0);
		rl_on_new_line();
		write(1, "\n", 1);
		rl_redisplay();
	}
}

void	setup_signal_handling()
{
	g_received_signal = 0;
	signal(SIGINT, handle_signal);
	signal(SIGQUIT, SIG_IGN);
}

void	setup_heredoc_signal_handling()
{
	g_received_signal = 0;
	signal(SIGINT, handle_heredoc_signal);
	signal(SIGQUIT, SIG_IGN);
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