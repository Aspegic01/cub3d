/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_s.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlabrirh <mlabrirh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/24 21:00:48 by mlabrirh          #+#    #+#             */
/*   Updated: 2025/12/26 16:21:01 by mlabrirh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

int	check_first_last_row(t_map *map, int row)
{
	int	j;

	j = 0;
	while (j < (int)ft_strlen(map->grid[row]))
	{
		if (map->grid[row][j] != '1' && map->grid[row][j] != ' ')
			return (0);
		j++;
	}
	return (1);
}

int	check_side_walls(t_map *map)
{
	int	i;
	int	len;

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

int	is_valid_neighbor(t_map *map, int row, int col)
{
	return (map->grid[row][col] == '1' || map->grid[row][col] == '0' ||
			map->grid[row][col] == 'N' || map->grid[row][col] == 'S' ||
			map->grid[row][col] == 'E' || map->grid[row][col] == 'W');
}

int	validate_walkable_spaces(t_map *map)
{
	int	i;
	int	j;

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
				if (!is_valid_neighbor(map, i - 1, j)
					|| !is_valid_neighbor(map, i + 1, j)
					|| !is_valid_neighbor(map, i, j - 1)
					|| !is_valid_neighbor(map, i, j + 1))
					return (ft_putstr_fd("Error\nMap not valid: \
							walkable space not properly enclosed\n", 2), 0);
			}
			j++;
		}
		i++;
	}
	return (1);
}

int	check_map_surrounded_by_walls(t_map *map)
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
