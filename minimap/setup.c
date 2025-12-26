/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setup.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: klaayoun <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/25 19:04:59 by klaayoun          #+#    #+#             */
/*   Updated: 2025/12/25 19:05:05 by klaayoun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../cub3d.h"

int32_t	clamp(int32_t value, int32_t min, int32_t max)
{
	if (value < min)
		return (min);
	if (value > max)
		return (max);
	return (value);
}

static void	map_print(t_map *map)
{
	size_t	i;
	size_t	j;

	i = 0;
	while (map->grid[i])
	{
		j = 0;
		while (map->grid[i][j])
			j++;
		write(1, map->grid[i], j);
		write(1, "\n", 1);
		i++;
	}
}

int	load_map_textures(t_map *map)
{
	map->tex_north = mlx_load_png(map->textures.north);
	map->tex_south = mlx_load_png(map->textures.south);
	map->tex_west = mlx_load_png(map->textures.west);
	map->tex_east = mlx_load_png(map->textures.east);
	if (!map->tex_north || !map->tex_south || !map->tex_west || !map->tex_east)
		return (ft_putstr_fd("Error\nFailed to load textures\n", 2), -1);
	return (0);
}

int	minimap_setup(t_game *game)
{
	t_v2i	grid_dimensions;
	int32_t	img_idx;

	grid_dimensions = veci_new(game->map->width, game->map->height);
	game->map->size = veci_new(CELL_SIZE * CELL_COUNT, CELL_SIZE * CELL_COUNT);
	game->map->img = mlx_new_image(game->mlx, game->map->size.x,
			game->map->size.y);
	if (!game->map->img)
		return (ft_putstr_fd("Error\nFailed to initialize image\n", 2), -1);
	game->map->position = veci_new(WIN_WIDTH * 0.01, WIN_HEIGHT * 0.01);
	img_idx = mlx_image_to_window(game->mlx, game->map->img,
			game->map->position.x, game->map->position.y);
	if (img_idx < 0)
		return (ft_putstr_fd("Error\nFailed to put image to window\n", 2), -1);
	if (load_map_textures(game->map) < 0)
		return (-1);
	map_print(game->map);
	veci_print("canvas", veci_new(WIN_WIDTH, WIN_HEIGHT));
	veci_print("grid", grid_dimensions);
	veci_print("map_size", game->map->size);
	veci_print("map_position", game->map->position);
	veci_print("cell_size", veci_new(CELL_SIZE, CELL_SIZE));
	vecf_print("player", game->map->player.position);
	return (0);
}
