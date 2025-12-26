/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_c.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlabrirh <mlabrirh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/24 21:03:57 by mlabrirh          #+#    #+#             */
/*   Updated: 2025/12/26 16:11:37 by mlabrirh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

int	store_map_line(char *line, t_map *map)
{
	int	len;
	int	i;

	len = ft_strlen(line);
	if (line[len - 1] == '\n')
		len--;
	map->grid[map->map_line_index] = malloc(sizeof(char) * (len + 1));
	if (!map->grid[map->map_line_index])
		return (0);
	i = 0;
	while (i < len)
	{
		map->grid[map->map_line_index][i] = line[i];
		i++;
	}
	map->grid[map->map_line_index][len] = '\0';
	if (len > map->width)
		map->width = len;
	return (1);
}

void	free_map_grid(t_map *map)
{
	int	i;

	if (!map || !map->grid)
		return ;
	i = 0;
	while (i < map->height && map->grid[i])
	{
		free(map->grid[i]);
		i++;
	}
	free(map->grid);
	map->grid = NULL;
}

int	is_valid_map_char(char c)
{
	return (c == '1' || c == '0' || c == 'N' || c == 'S'
		|| c == 'E' || c == 'W' || c == ' ');
}

int	load_map_data(char *map_file, t_map *map)
{
	int		fd;
	char	*line;

	fd = open(map_file, O_RDONLY);
	if (fd < 0)
		return (0);
	line = get_next_line(fd);
	while (line)
	{
		if (ft_strncmp(line, "\n", ft_strlen(line)) != 0)
		{
			if (map->line_count >= 6)
			{
				if (!store_map_line(line, map))
					return (free(line), close(fd), 0);
				map->map_line_index++;
			}
			map->line_count++;
		}
		free(line);
		line = get_next_line(fd);
	}
	return (close(fd), 1);
}
