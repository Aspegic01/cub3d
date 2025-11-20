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
	if (game->mlx)
		mlx_close_window(game->mlx);
	return (0);
}

int32_t xmove_player(t_map *map, int32_t value)
{
	int32_t box = map->cell_size * 0.5;
	int32_t y1 = map->player_position.y / map->cell_size;
	int32_t y2 = (map->player_position.y + box) / map->cell_size;

	if (value > 0)
	{
		int32_t x = (map->player_position.x + box + value) / map->cell_size;
		if (map->grid[y1][x] == '0' && map->grid[y2][x] == '0')
			map->player_position.x += value;
	} else
	{
		int32_t x = (map->player_position.x + value) / map->cell_size;
		if (map->grid[y1][x] == '0' && map->grid[y2][x] == '0')
			map->player_position.x += value;
	}
	return map->player_position.x;
}

int32_t ymove_player(t_map *map, int32_t value)
{
	int32_t box = map->cell_size * 0.5;
	int32_t x1 = map->player_position.x / map->cell_size;
	int32_t x2 = (map->player_position.x + box) / map->cell_size;

	if (value > 0)
	{
		int32_t y = (map->player_position.y + box + value) / map->cell_size;
		if (map->grid[y][x1] == '0' &&  map->grid[y][x2] == '0')
			map->player_position.y += value;
	}
	else
	{
		int32_t y = (map->player_position.y + value) / map->cell_size;
		if (map->grid[y][x1] == '0' && map->grid[y][x2] == '0') {
			map->player_position.y += value;
		}
	}
	return map->player_position.y;
}

static void	rotate_player(t_map *map, double angle)
{
	double	old_dir_x;
	double	old_plane_x;
	double	cos_a;
	double	sin_a;

	old_dir_x = map->player.dir_x;
	old_plane_x = map->player.plane_x;
	cos_a = cos(angle);
	sin_a = sin(angle);
	map->player.dir_x = old_dir_x * cos_a - map->player.dir_y * sin_a;
	map->player.dir_y = old_dir_x * sin_a + map->player.dir_y * cos_a;
	map->player.plane_x = old_plane_x * cos_a - map->player.plane_y * sin_a;
	map->player.plane_y = old_plane_x * sin_a + map->player.plane_y * cos_a;
}

void	capture_keys(void *param)
{
	t_game	*game;
	double	rot_speed;

	game = param;
	rot_speed = 0.05;
	if (mlx_is_key_down(game->mlx, MLX_KEY_ESCAPE))
		close_window(game);
	if (mlx_is_key_down(game->mlx, MLX_KEY_D))
		game->map->player_position.x = xmove_player(game->map, 1);
	if (mlx_is_key_down(game->mlx, MLX_KEY_A))
		game->map->player_position.x = xmove_player(game->map, -1);
	if (mlx_is_key_down(game->mlx, MLX_KEY_S))
		game->map->player_position.y = ymove_player(game->map, 1);
	if (mlx_is_key_down(game->mlx, MLX_KEY_W))
		game->map->player_position.y = ymove_player(game->map, -1);
	if (mlx_is_key_down(game->mlx, MLX_KEY_LEFT))
		rotate_player(game->map, -rot_speed);
	if (mlx_is_key_down(game->mlx, MLX_KEY_RIGHT))
		rotate_player(game->map, rot_speed);
}

static void	start(void *param)
{
	t_game	*game;

	game = param;
	render_game(game);
	minimap_render(game->map);
}

int	init_window(t_game *game)
{
	game->mlx = mlx_init(16 * 100, 9 * 100, WIN_TITLE, false);
	if (!game->mlx)
		return (ft_putstr_fd("Error\nFailed to initialize MLX\n", 2), -1);
	game->canvas = mlx_new_image(game->mlx, game->mlx->width, game->mlx->height);
	if (!game->mlx)
		return (ft_putstr_fd("Error\nFailed to init image\n", 2), -1);
	if (mlx_image_to_window(game->mlx, game->canvas, 0, 0) < 0)
		return (ft_putstr_fd("Error\nFailed to put image to window\n", 2), -1);
	if (minimap_setup(game) != 0)
		return (-1);
	mlx_loop_hook(game->mlx, capture_keys, game);
	mlx_loop_hook(game->mlx, start, game);
	return (0);
}
