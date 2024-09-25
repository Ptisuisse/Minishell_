#include "minishell.h"

void	handle_signal(int sig)
{
	if (sig == SIGINT) //<<------- Autre soluce [ctrl +c ] sig 2 ?
		write(1, "\nMinishell > ", 13);
	else if (sig == SIGSEGV)
		exit(1); // <<-------- FREE [ctrl + d] sig 11
	return ;
}

int	signal_handle(void)
{
	struct sigaction sa;

	sa.sa_handler = handle_signal;
	sa.sa_flags = 0;
	sigemptyset(&sa.sa_mask);
	sigaction(SIGINT, &sa, NULL);
	sigaction(SIGSEGV, &sa, NULL); // ctrl + d
	sa.sa_handler = SIG_IGN;
	sigaction(SIGQUIT, &sa, NULL);
	return (0);
}