/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lvan-slu <lvan-slu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/13 14:24:43 by lvan-slu          #+#    #+#             */
/*   Updated: 2024/03/03 12:11:33 by lvan-slu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*sorting_buf(char *buf)
{
	int		i;
	int		j;
	char	*line;

	i = 0;
	while (buf[i] && buf[i] != '\n')
		i++;
	if (!buf[i] || (buf[i] == '\n' && buf[i + 1] == '\n'))
	{
		free(buf);
		return (NULL);
	}
	line = ft_calloc((ft_strlen(buf) - i + 1), (sizeof(char)));
	i++;
	j = 0;
	while (buf[i])
		line[j++] = buf[i++];
	free(buf);
	return (line);
}

char	*sorting_line(char *tmp)
{
	char	*str;
	int		i;

	i = 0;
	if (!tmp[i])
		return (NULL);
	while (tmp[i] && tmp[i] != '\n')
		i++;
	str = ft_calloc((i + 2), sizeof(char));
	i = 0;
	while (tmp[i] && tmp[i] != '\n')
	{
		str[i] = tmp[i];
		i++;
	}
	if (tmp[i] && tmp[i] == '\n')
		str[i] = '\n';
	return (str);
}

char	*read_line(int fd, char *buf)
{
	char	*str;
	char	*tmp_2;
	int		i;

	tmp_2 = ft_strdup(buf);
	str = ft_calloc((BUFFER_SIZE + 1), sizeof(char));
	if (!str || !buf)
		return (NULL);
	i = 1;
	while (i > 0)
	{
		i = read(fd, str, BUFFER_SIZE);
		if (i == -1)
		{
			free(str);
			free(tmp_2);
			return (NULL);
		}
		str[i] = '\0';
		tmp_2 = free_strjoin(tmp_2, str);
		if (ft_strrchr(tmp_2, '\n'))
			break ;
	}
	free(str);
	return (tmp_2);
}

char	*get_next_line(int fd)
{
	static char	*buf;
	char		*tmp;
	char		*line;

	if (!buf)
		buf = ft_calloc(1, 1);
	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	tmp = read_line(fd, buf);
	if (!tmp)
		return (NULL);
	line = sorting_line(tmp);
	free(buf);
	buf = sorting_buf(tmp);
	return (line);
}
