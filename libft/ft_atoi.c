/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lvan-slu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/10 22:10:32 by lvan-slu          #+#    #+#             */
/*   Updated: 2023/11/19 19:19:55 by lvan-slu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_atoi(const char *str)
{
	int	i;
	int	j;
	int	k;

	i = 0;
	j = 1;
	k = 0;
	while (str[i] != '\0')
	{
		while (str[i] == 32 || (str[i] >= 9 && str[i] <= 13))
			i++;
		if (str[i] == 43 || str [i] == 45)
		{
			if (str[i] == 45)
				j *= -1;
			i++;
		}
		while (str[i] >= '0' && str [i] <= '9')
		{
			k = k * 10 + (str[i] - '0');
			i++;
		}
		return (k * j);
	}
	return (0);
}
