/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lvan-slu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/17 13:05:12 by lvan-slu          #+#    #+#             */
/*   Updated: 2024/10/17 13:05:14 by lvan-slu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*replace_by_exit_code(char *result, int *result_index)
{
	char	*exit_code;
	int		i;

	i = 0;
	exit_code = ft_itoa(g_exit_code);
	while (exit_code[i])
		result[(*result_index)++] = exit_code[i++];
	free(exit_code);
	return (result);
}
