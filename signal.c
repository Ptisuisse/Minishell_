/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lvan-slu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/22 12:47:53 by lvan-slu          #+#    #+#             */
/*   Updated: 2024/10/22 12:47:54 by lvan-slu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int g_received_signal; // Global variable to store the received signal number

void	handle_signal(int sig)
{
	g_received_signal = sig; // Store the signal number in the global variable

	if (sig == SIGINT)
	{
		rl_clear_history();
		write(1, "\nMiniBash > ", 13);
	}
	else if (sig == SIGSEGV)
		exit(1);
}

int	signal_handle()
{
	struct sigaction	sa;

	sa.sa_handler = handle_signal; // Set the signal handler function
	sa.sa_flags = 0;
	sigemptyset(&sa.sa_mask);

	sigaction(SIGINT, &sa, NULL);  // Handle SIGINT (Ctrl+C)
	sigaction(SIGSEGV, &sa, NULL); // Handle SIGSEGV (segmentation fault)

	sa.sa_handler = SIG_IGN;       // Ignore SIGQUIT
	sigaction(SIGQUIT, &sa, NULL);

	return (0);
}

void	handle_signals_in_command(t_command *command_list)
{
	if (g_received_signal == SIGINT)
	{
		command_list->exit_code = 130; // Handle SIGINT in the context of the command
	}
	// You can handle other signals if needed here
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
