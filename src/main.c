/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lisambet <lisambet@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/17 13:05:26 by lvan-slu          #+#    #+#             */
/*   Updated: 2024/11/20 13:22:24 by lisambet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int		g_received_signal = 0;

int	main(int argc, char **argv, char **envp)
{
	t_command	*command_list;
	t_env		*env_list;
	char		*input;
	int			save_exit_code;

	input = NULL;
	save_exit_code = 0;
	(void)argv;
	if (argc != 1)
	{
		ft_printf("Error");
		return (1);
	}
	create_env_list(envp, &env_list);
	command_list = NULL;
	ft_prompt(save_exit_code, command_list, &env_list, input);
	free_env_list(&env_list);
	return (0);
}
