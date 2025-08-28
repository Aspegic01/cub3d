/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlabrirh <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/07 19:00:51 by mlabrirh          #+#    #+#             */
/*   Updated: 2024/12/07 19:17:30 by mlabrirh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line_bonus.h"

char	*extract_line(char **buffer)
{
	char	*newline_pos;
	char	*line;
	char	*new_buffer;
	size_t	len;

	newline_pos = ft_strchr(*buffer, '\n');
	if (newline_pos)
	{
		len = newline_pos - *buffer + 1;
		line = ft_substr(*buffer, 0, len);
		if (!line)
			return (free(*buffer), free(line), NULL);
		new_buffer = ft_strdup(newline_pos + 1);
		free(*buffer);
		*buffer = new_buffer;
	}
	else
	{
		line = *buffer;
		*buffer = NULL;
	}
	return (line);
}

char	*read_from_fd(int fd, char **buffer)
{
	char	*new_buffer;
	char	*temp;
	ssize_t	bytes_read;

	temp = malloc((size_t)BUFFER_SIZE + 1);
	if (!temp)
		return (NULL);
	while (!ft_strchr(*buffer, '\n'))
	{
		bytes_read = read(fd, temp, BUFFER_SIZE);
		if (bytes_read < 0)
			return (free(temp), NULL);
		if (bytes_read == 0)
			break ;
		temp[bytes_read] = '\0';
		new_buffer = ft_strjoin(*buffer, temp);
		free(*buffer);
		if (!new_buffer)
			return (free(temp), free(*buffer), NULL);
		*buffer = new_buffer;
	}
	free(temp);
	return (*buffer);
}

char	*get_next_line(int fd)
{
	static char	*buffer[FD];

	if (fd < 0 || BUFFER_SIZE <= 0 || fd > FD)
		return (NULL);
	if (!buffer[fd])
		buffer[fd] = ft_strdup("");
	if (!read_from_fd(fd, &buffer[fd]))
		return (NULL);
	if (!buffer[fd] || buffer[fd][0] == '\0')
	{
		free(buffer[fd]);
		buffer[fd] = NULL;
		return (NULL);
	}
	return (extract_line(&buffer[fd]));
}
