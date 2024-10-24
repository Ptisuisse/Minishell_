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

int g_received_signal = 0;

void	handle_signal(int sig)
{
	g_received_signal = sig; // Store the signal number in the global variable

	if (sig == SIGINT || sig == SIGQUIT)
	{
		//signal_handle();
		rl_clear_history();
		write(1, "\nMiniBash > ", 13);
	}
	else if (sig == SIGSEGV)
		exit(1);
}

void	setup_signal_handling()
{
	g_received_signal = 0;
	signal(SIGINT, handle_signal); 
	signal(SIGQUIT, SIG_IGN);
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
