/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gnl.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncrivell <ncrivell@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/23 14:46:14 by ncrivell          #+#    #+#             */
/*   Updated: 2025/09/18 12:41:03 by ncrivell         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include "libft.h"
#include <stdlib.h>
#include "cub3d.h"

#ifndef BUFFER_SIZE
#define BUFFER_SIZE 42
#endif
char	*ft_explore_text(char *principal_buff, int fd)
{
	char	*buff_temp;
	char	*buff_reader;
	int		bytes;

	bytes = 1;
	buff_reader = malloc((BUFFER_SIZE + 1) * sizeof(char));
	if (!buff_reader)
		return (freebuff(&principal_buff));
	buff_reader[0] = '\0';
	while (!ft_strchr(buff_reader, '\n') && bytes != 0)
	{
		bytes = read(fd, buff_reader, BUFFER_SIZE);
		if (bytes < 0)
			return (freebuff(&principal_buff));
		buff_reader[bytes] = '\0';
		buff_temp = ft_strjoin(principal_buff, buff_reader);
		if (!buff_temp)
			return (freebuff(&principal_buff));
		free(principal_buff);
		principal_buff = buff_temp;
	}
	free(buff_reader);
	return (principal_buff);
}

char	*ft_create_line(char *principal_buff)
{
	char	*line_created;
	int		i;

	i = 0;
	while (principal_buff[i] && principal_buff[i] != '\n')
		i++;
	line_created = malloc((i + 1 + (principal_buff[i] == '\n')) * sizeof(char));
	if (!line_created)
		return (freebuff(&principal_buff));
	i = 0;
	while (principal_buff[i] && principal_buff[i] != '\n')
	{
		line_created[i] = principal_buff[i];
		i++;
	}
	if (principal_buff[i] == '\n')
	{
		line_created[i] = principal_buff[i];
		i++;
	}
	line_created[i] = '\0';
	return (line_created);
}

char	*ft_new_buff(char *principal_buff)
{
	char		*new_temp;
	size_t		memo_len;
	size_t		i;
	int			j;

	j = 0;
	i = 0;
	while (principal_buff[i] && principal_buff[i] != '\n' )
		i++;
	if (!principal_buff[i])
		return (freebuff(&principal_buff));
	memo_len = ft_strlen(principal_buff);
	new_temp = malloc((memo_len - i) * sizeof(char));
	if (!new_temp)
		return (freebuff(&principal_buff));
	i++;
	while (principal_buff[i])
	{
		new_temp[j] = principal_buff[i];
		j++;
		i++;
	}
	new_temp[j] = '\0';
	free(principal_buff);
	return (new_temp);
}

char	*freebuff(char **principal_buff)
{
	free(*principal_buff);
	*principal_buff = NULL;
	return (NULL);
}

char	*get_next_line(int fd)
{
	static char	*principal_buff;
	char		*write_line;

	if (fd < 0 || BUFFER_SIZE <= 0 || read(fd, NULL, 0) < 0)
		return (freebuff(&principal_buff));
	if (!principal_buff)
		principal_buff = ft_strdup("");
	if (!principal_buff)
		return (freebuff(&principal_buff));
	if (!ft_strchr(principal_buff, '\n'))
		principal_buff = ft_explore_text(principal_buff, fd);
	if (!principal_buff || principal_buff[0] == '\0')
		return (freebuff(&principal_buff));
	write_line = ft_create_line(principal_buff);
	if (!write_line)
		return (freebuff(&principal_buff));
	principal_buff = ft_new_buff(principal_buff);
	if (!principal_buff)
	{
		free(principal_buff);
		principal_buff = NULL;
	}
	return (write_line);
}
