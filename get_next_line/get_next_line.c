/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlabrirh <mlabrirh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/27 12:42:11 by mlabrirh          #+#    #+#             */
/*   Updated: 2025/10/25 11:40:02 by mlabrirh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"
static char *gnl_buffer = NULL;

void	gnl_free_buffer(void)
{
	if (gnl_buffer)
	{
		free(gnl_buffer);
		gnl_buffer = NULL;
	}
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
			return (free(temp), NULL);
		*buffer = new_buffer;
	}
	free(temp);
	return (*buffer);
}

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
			return (free(*buffer), NULL);
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

char	*get_next_line(int fd)
{
	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	if (!gnl_buffer)
		gnl_buffer = ft_strdup("");
	if (!read_from_fd(fd, &gnl_buffer))
		return (NULL);
	if (!gnl_buffer || gnl_buffer[0] == '\0')
	{
		free(gnl_buffer);
		gnl_buffer = NULL;
		return (NULL);
	}
	return (extract_line(&gnl_buffer));
}
