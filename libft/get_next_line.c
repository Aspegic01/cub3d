/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlabrirh <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/02 17:25:58 by mlabrirh          #+#    #+#             */
/*   Updated: 2026/01/02 17:26:57 by mlabrirh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static void	*cleanup(char **buf)
{
	if (*buf != NULL)
		free(*buf);
	*buf = NULL;
	return (NULL);
}

static char	*get_line(char **p_buf, size_t end)
{
	char	*tmp;
	char	*buf;
	size_t	i;

	buf = *p_buf;
	i = 0;
	tmp = ft_strndup(buf, end);
	if (!tmp)
		return (cleanup(p_buf));
	while (buf[end])
		buf[i++] = buf[end++];
	while (buf[i])
		buf[i++] = '\0';
	return (tmp);
}

static char	*expand_buf(char **buf, size_t buf_size)
{
	char			*new_buf;
	size_t			i;
	unsigned long	read_buf_size;

	i = 0;
	read_buf_size = (unsigned long)BUFFER_SIZE;
	new_buf = (char *)malloc(buf_size + read_buf_size + 1);
	if (!new_buf)
		return (cleanup(buf));
	while ((*buf)[i])
	{
		new_buf[i] = (*buf)[i];
		i++;
	}
	new_buf[i] = '\0';
	free(*buf);
	*buf = new_buf;
	return (new_buf);
}

static ssize_t	process_buf(int fd, char **buf, ssize_t bytes, char **rec)
{
	ssize_t	buf_size;
	ssize_t	nl_pos;

	nl_pos = ft_istrchr(*buf, '\n');
	while (nl_pos == -1)
	{
		buf_size = ft_strlen(*buf);
		if (!expand_buf(buf, buf_size))
			return (cleanup(buf), -1);
		bytes = read(fd, *buf + buf_size, BUFFER_SIZE);
		(*buf)[buf_size + bytes] = '\0';
		if (bytes == 0)
			return (*rec = get_line(buf, buf_size), buf_size + bytes);
		if (bytes == -1)
			return (cleanup(buf), -1);
		nl_pos = ft_istrchr(*buf, '\n');
	}
	return (*rec = get_line(buf, nl_pos + 1), nl_pos + 1);
}

ssize_t	get_next_line(int fd, char **receiver)
{
	static char		*buf;
	ssize_t			bytes;
	unsigned long	read_buf_size;

	if (fd < 0 || BUFFER_SIZE <= 0)
		return (-1);
	read_buf_size = (unsigned long)BUFFER_SIZE;
	bytes = 0;
	if (!buf)
	{
		buf = (char *)malloc(read_buf_size + 1);
		if (!buf)
			return (-1);
		buf[0] = '\0';
	}
	if (!buf[0])
	{
		bytes = read(fd, buf, BUFFER_SIZE);
		if (bytes == -1 || bytes == 0)
			return (cleanup(&buf), bytes);
		buf[bytes] = '\0';
	}
	return (process_buf(fd, &buf, bytes, receiver));
}
