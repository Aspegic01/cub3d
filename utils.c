/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlabrirh <mlabrirh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/28 13:10:00 by mlabrirh          #+#    #+#             */
/*   Updated: 2025/10/25 14:48:02 by mlabrirh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

int allocate_map_grid(t_map *map)
{
	int i;

	map->grid = malloc(sizeof(char *) * (map->height + 1));
	if (!map->grid)
		return (0);
	i = 0;
	while (i <= map->height)
	{
		map->grid[i] = NULL;
		i++;
	}
	return (1);
}

int store_map_line(char *line, t_map *map)
{
	int len;
	int i;

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

void free_map_grid(t_map *map)
{
	int i;

	if (!map || !map->grid)
		return;
	i = 0;
	while (i < map->height && map->grid[i])
	{
		free(map->grid[i]);
		i++;
	}
	free(map->grid);
	map->grid = NULL;
}

int is_valid_map_char(char c)
{
	return (c == '1' || c == '0' || c == 'N' || c == 'S' ||
			c == 'E' || c == 'W' || c == ' ');
}

int check_first_last_row(t_map *map, int row)
{
	int j;

	j = 0;
	while (j < (int)ft_strlen(map->grid[row]))
	{
		if (map->grid[row][j] != '1' && map->grid[row][j] != ' ')
			return (0);
		j++;
	}
	return (1);
}

int check_side_walls(t_map *map)
{
	int i;
	int len;

	i = 0;
	while (i < map->height)
	{
		len = ft_strlen(map->grid[i]);
		if (len > 0)
		{
			if (map->grid[i][0] != '1' && map->grid[i][0] != ' ')
				return (0);
			if (map->grid[i][len - 1] != '1' && map->grid[i][len - 1] != ' ')
				return (0);
		}
		i++;
	}
	return (1);
}

int is_valid_neighbor(t_map *map, int row, int col)
{
	if (row < 0 || row >= map->height || col < 0)
		return (0);
	if (col >= (int)ft_strlen(map->grid[row]))
		return (0);
	return (map->grid[row][col] == '1' || map->grid[row][col] == '0' ||
			map->grid[row][col] == 'N' || map->grid[row][col] == 'S' ||
			map->grid[row][col] == 'E' || map->grid[row][col] == 'W');
}

int validate_walkable_spaces(t_map *map)
{
	int i;
	int j;

	i = 0;
	while (i < map->height)
	{
		j = 0;
		while (j < (int)ft_strlen(map->grid[i]))
		{
			if (map->grid[i][j] == '0' || map->grid[i][j] == 'N' ||
				map->grid[i][j] == 'S' || map->grid[i][j] == 'E' ||
				map->grid[i][j] == 'W')
			{
				if (!is_valid_neighbor(map, i - 1, j) ||
					!is_valid_neighbor(map, i + 1, j) ||
					!is_valid_neighbor(map, i, j - 1) ||
					!is_valid_neighbor(map, i, j + 1))
					return (ft_putstr_fd("Error\nMap not valid: \
							walkable space not properly enclosed\n", 2), 0);
			}
			j++;
		}
		i++;
	}
	return (1);
}

int check_map_surrounded_by_walls(t_map *map)
{
	if (!map || !map->grid || map->height == 0)
		return (0);
	if (!check_first_last_row(map, 0))
		return (ft_putstr_fd("Error\nMap not closed: first row\n", 2), 0);
	if (!check_first_last_row(map, map->height - 1))
		return (ft_putstr_fd("Error\nMap not closed: last row\n", 2), 0);
	if (!check_side_walls(map))
		return (ft_putstr_fd("Error\nMap not closed: side walls\n", 2), 0);
	if (!validate_walkable_spaces(map))
		return (0);
	return (1);
}

int load_map_data(char *map_file, t_map *map)
{
	int fd;
	char *line;

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

static int is_walkable_char(char c)
{
	return (c == '0' || c == 'N' || c == 'S' || c == 'E' || c == 'W');
}

static int is_space_adjacent_to_walkable(t_map *map, int i, int j)
{
	if (i > 0 && is_walkable_char(map->grid[i - 1][j]))
		return (1);
	if (i < map->height - 1 && is_walkable_char(map->grid[i + 1][j]))
		return (1);
	if (j > 0 && is_walkable_char(map->grid[i][j - 1]))
		return (1);
	if (map->grid[i][j + 1] && is_walkable_char(map->grid[i][j + 1]))
		return (1);
	return (0);
}

static int process_spaces_once(t_map *map)
{
	int i, j;
	int changed;

	changed = 0;
	i = 0;

	while (i < map->height)
	{
		j = 0;
		while (j < map->width && map->grid[i][j])
		{
			if (map->grid[i][j] == ' ' && is_space_adjacent_to_walkable(map, i, j))
			{
				map->grid[i][j] = '0';
				changed = 1;
			}
			j++;
		}
		i++;
	}

	return (changed);
}

void fix_zero_space_to_zero(t_map *map)
{
	int changed;

	changed = 1;
	while (changed)
	{
		changed = process_spaces_once(map);
	}
}
