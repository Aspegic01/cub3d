/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_parsing_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlabrirh <mlabrirh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/24 20:57:07 by mlabrirh          #+#    #+#             */
/*   Updated: 2025/12/24 21:10:57 by mlabrirh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

t_map	*init_map(void)
{
	t_map	*map;

	map = malloc(sizeof(t_map));
	if (!map)
		return (NULL);
	map->grid = NULL;
	map->width = 0;
	map->height = 0;
	map->map_line_index = 0;
	map->line_count = 0;
	map->player_count = 0;
	map->elements_count = 0;
	map->textures.north = NULL;
	map->textures.south = NULL;
	map->textures.west = NULL;
	map->textures.east = NULL;
	map->colors.floor_r = -1;
	map->colors.floor_g = -1;
	map->colors.floor_b = -1;
	map->colors.ceiling_r = -1;
	map->colors.ceiling_b = -1;
	map->colors.ceiling_g = -1;
	return (map);
}

int	validate_final_map(t_map *map)
{
	if (map->elements_count != 6)
	{
		free_textures(map);
		free(map);
		return (ft_putstr_fd("Error\nMissing elements \
            (need NO, SO, WE, EA, F, C)\n", 2), -1);
	}
	if (map->height == 0)
	{
		free_textures(map);
		free(map);
		return (ft_putstr_fd("Error\nNo map found\n", 2), -1);
	}
	return (0);
}
