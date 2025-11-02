/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   window.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlabrirh <mlabrirh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/04 11:30:00 by mlabrirh          #+#    #+#             */
/*   Updated: 2025/10/17 11:23:23 by mlabrirh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

int	close_window(t_game *game)
{
	// if (game->map)
	// {
	// 	free_textures(game->map);
	// 	free_map_grid(game->map);
	// 	free(game->map);
	// }
	if (game->mlx)
		mlx_close_window(game->mlx);
	return (0);
}

void	capture_keys(void *param)
{
	t_game	*game;

	game = param;
	if (mlx_is_key_down(game->mlx, MLX_KEY_ESCAPE))
		close_window(game);
}

static inline int32_t	clamp_range(int32_t x, int32_t min, int32_t max)
{
	if (x < min)
		return (min);
	if (x > max)
		return (max);
	return (x);
}

void	render_map_cell(t_map *scene, t_v2 pos, uint32_t color)
{
	int32_t	x0;
	int32_t	y0;
	int32_t	x1;
	int32_t	y1;

	x0 = clamp_range(pos.x, 0, scene->minimap->width);
	y0 = clamp_range(pos.y, 0, scene->minimap->height);
	x1 = clamp_range(pos.x + scene->cell_size, 0, scene->minimap->width);
	y1 = clamp_range(pos.y + scene->cell_size, 0, scene->minimap->height);
	for (int32_t px = x0; px < x1; px++)
	{
		for (int32_t py = y0; py < y1; py++)
		{
			mlx_put_pixel(scene->minimap, px, py, color);
		}
	}
}

void	render_minimap(t_map *scene)
{
	t_v2	iter;

	// t_v2	pos;
	// pos = vec_zero();
	iter = vec_zero();
	while (scene->grid[iter.x])
	{
		iter.y = 0;
		while (scene->grid[iter.y])
		{
			// pos.x = iter.x * scene->cell_size;
			// pos.y = iter.y * scene->cell_size;
			if (scene->grid[iter.x][iter.y] == '0')
			{
				// render_map_cell(scene, iter, 0x333333);
				// mlx_put_pixel(scene->minimap, iter.x, iter.y, 0x333333);
			}
			else
			{
				mlx_put_pixel(scene->minimap, iter.x, iter.y, 0xFFFFFFFF);
				// render_map_cell(scene, iter, 0xFFFFFFFF);
			}
			iter.y++;
		}
		iter.x++;
	}
}

static void	start(void *param)
{
	t_game	*game;

	game = param;
	render_minimap(game->map);
}

int	init_window(t_game *game)
{
	t_v2	msize;
	t_v2	scene_size;
	int32_t	instance_idx;

	game->mlx = mlx_init(1920, 1080, WIN_TITLE, false);
	if (!game->mlx)
		return (ft_putstr_fd("Error\nFailed to initialize MLX\n", 2), -1);
	game->map->cell_size = 1920 * 0.02;
	scene_size = vec_new(game->map->width, game->map->height);
	msize = vec_scale(scene_size, game->map->cell_size);
	game->map->minimap = mlx_new_image(game->mlx, msize.x, msize.y);
	if (!game->map->minimap)
		return (ft_putstr_fd("Error\nFailed to initialize image\n", 2), -1);
	instance_idx = mlx_image_to_window(game->mlx, game->map->minimap, 0, 0);
	if (instance_idx < 0)
		return (ft_putstr_fd("Error\nFailed to put image\n", 2), -1);
	printf("cell_size %d\n", game->map->cell_size);
	vec_print("scene_size", scene_size);
	vec_print("minimap_size", msize);
	mlx_loop_hook(game->mlx, capture_keys, game);
	mlx_loop_hook(game->mlx, start, game);
	return (0);
}
