/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lisambet <lisambet@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/06 11:53:39 by lvan-slu          #+#    #+#             */
/*   Updated: 2024/11/20 13:17:38 by lisambet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	handle_signal_child(int sig)
{
	if (sig == SIGINT)
	{
		g_received_signal = 11;
		close(0);
	}
}

void	handle_signal_parent(int sig)
{
	if (sig == SIGINT)
	{
		g_received_signal = 11;
	}
}

void	handle_signal(int sig)
{
	if (sig == SIGINT)
	{
		write(1, "\n", 1);
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
	}
}

void	setup_signal_handling(void)
{
	signal(SIGINT, handle_signal);
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
	return (0);
}
