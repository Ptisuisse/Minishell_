/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_strjoin.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lvan-slu <lvan-slu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/03 12:10:04 by lvan-slu          #+#    #+#             */
/*   Updated: 2024/03/03 12:12:22 by lvan-slu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*free_strjoin(char *s1, char *s2)
{
	int		ls1;
	int		ls2;
	char	*news;

	if (!s1 || !s2)
		return (0);
	ls1 = ft_strlen(s1);
	ls2 = ft_strlen(s2);
	news = ft_calloc((ls1 + ls2 + 1), sizeof(char));
	if (news == NULL)
		return (NULL);
	ft_memcpy(news, s1, ls1);
	ls2 = 0;
	while (s2[ls2] != '\0')
	{
		news[ls1] = s2[ls2];
		ls1++;
		ls2++;
	}
	free(s1);
	return (news);
}
