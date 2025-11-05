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
	int32_t	px;
	int32_t	py;

	x0 = pos.x;
	y0 = pos.y;
	x1 = pos.x + scene->cell_size;
	y1 = pos.y + scene->cell_size;
	px = x0;
	while (px < x1)
	{
		py = y0;
		while (py < y1)
		{
			mlx_put_pixel(scene->img, px, py, color);
			py++;
		}
		px++;
	}
}

void	render_minimap(t_map *scene)
{
	t_v2	iter;
	t_v2	pos;

	iter = vec_zero();
	while (iter.y < scene->height)
	{
		iter.x = 0;
		while (iter.x <= scene->width)
		{
			pos = vec_scale(iter, scene->cell_size);
			if (scene->grid[iter.y][iter.x] == '0')
				render_map_cell(scene, pos, 0xFFFFFFFF );
			else 
				render_map_cell(scene, pos, 0x333333);
			iter.x++;
		}
		iter.y++;
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
	t_v2	grid_dimensions;
	int32_t	instance_idx;

	game->mlx = mlx_init(16 * 100, 9 * 100, WIN_TITLE, false);
	if (!game->mlx)
		return (ft_putstr_fd("Error\nFailed to initialize MLX\n", 2), -1);
	grid_dimensions = vec_new(game->map->width, game->map->height);
	game->map->cell_size = 1920 * 0.01;
	msize = vec_scale(grid_dimensions, game->map->cell_size);
	game->map->img = mlx_new_image(game->mlx, msize.x, msize.y);
	if (!game->map->img)
		return (ft_putstr_fd("Error\nFailed to initialize image\n", 2), -1);
	instance_idx = mlx_image_to_window(game->mlx, game->map->img, 0, 0);
	if (instance_idx < 0)
		return (ft_putstr_fd("Error\nFailed to put image\n", 2), -1);
	vec_print("grid", grid_dimensions);
	vec_print("map_size", msize);
	printf("cell_size %d\n", game->map->cell_size);
	game->map->run = true;
	mlx_loop_hook(game->mlx, capture_keys, game);
	mlx_loop_hook(game->mlx, start, game);
	return (0);
}
