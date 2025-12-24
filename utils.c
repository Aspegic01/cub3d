/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlabrirh <mlabrirh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/28 13:10:00 by mlabrirh          #+#    #+#             */
/*   Updated: 2025/12/24 21:04:22 by mlabrirh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

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
