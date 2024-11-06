/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lvan-slu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/18 14:52:40 by lvan-slu          #+#    #+#             */
/*   Updated: 2023/11/19 13:30:57 by lvan-slu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static	void	free_tab(char **tab)
{
	int	i;

	i = 0;
	while (tab[i])
	{
		free(tab[i]);
		i++;
	}
	free(tab);
	tab = NULL;
}

static int	count_sep(char const *s, char c)
{
	int	i;
	int	j;

	j = 0;
	i = 0;
	if (s[0] != c && s[0] != '\0')
		j++;
	while (s[i] != '\0')
	{
		if (s[i] == c && (s[i + 1] != c) && (s[i + 1] != '\0'))
			j++;
		i++;
	}
	return (j);
}

static char	**word_cpy(char const *s, char c, char **tab)
{
	size_t	i;
	size_t	j;

	j = 0;
	while (*s)
	{
		if (*s != c)
		{
			i = 0;
			while (s[i] != c && s[i] != '\0')
				i++;
			tab[j] = ft_substr(s, 0, i);
			if (!tab[j])
			{
				free_tab(tab);
				break ;
			}
			s = (s + i);
			j++;
		}
		else
			s++;
	}
	return (tab);
}

char	**ft_split(char const *s, char c)
{
	char	**split;

	if (!s)
		return (NULL);
	split = ft_calloc(((count_sep(s, c)) + 1), sizeof(char *));
	if (split == NULL)
		return (NULL);
	word_cpy(s, c, split);
	return (split);
}
