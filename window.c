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

static void	move_player_in_dir(t_map *map, t_v2f dir)
{
	t_v2f	new;
	int32_t	x1, x2, y1, y2;
	float_t	move_speed;

	move_speed = 0.5;
	new = vecf_add(map->player.position, vecf_scale(dir, move_speed));
	x1 = new.x / map->cell_size;
	x2 = (new.x + map->player_size) / map->cell_size;
	y1 = new.y / map->cell_size;
	y2 = (new.y + map->player_size) / map->cell_size;
	
	if (y1 >= 0 && y2 < map->height && x1 >= 0 && x2 < map->width)
	{
		int line1 = ft_strlen(map->grid[y1]);
		if (x1 >= line1)
			return ;
		int line2 = ft_strlen(map->grid[y2]);
		if (x2 >= line2)
			return ;
		if (map->grid[y1][x1] == '0' && map->grid[y1][x2] == '0' && map->grid[y2][x1] == '0' && map->grid[y2][x2] == '0')
		{
			map->player.position = new;
		}
	}
}

static void	rotate_player(t_map *map, double angle)
{
	float_t	cos_a;
	float_t	sin_a;
	t_v2f	new_dir;
	t_v2f	new_plane;

	cos_a = cosf(angle);
	sin_a = sinf(angle);
	new_dir.x = map->player.dir.x * cos_a - map->player.dir.y * sin_a;
	new_dir.y = map->player.dir.x * sin_a + map->player.dir.y * cos_a;
	new_plane.x = map->player.plane.x * cos_a - map->player.plane.y * sin_a;
	new_plane.y = map->player.plane.x * sin_a + map->player.plane.y * cos_a;
	map->player.dir = new_dir;
	map->player.plane = new_plane;
}

void	capture_keys(void *param)
{
	t_game	*game;
	double	rot_speed;

	game = param;
	rot_speed = 0.05;
	if (mlx_is_key_down(game->mlx, MLX_KEY_ESCAPE))
		close_window(game);
	if (mlx_is_key_down(game->mlx, MLX_KEY_W))	
		move_player_in_dir(game->map, game->map->player.dir);
	if (mlx_is_key_down(game->mlx, MLX_KEY_S))
		move_player_in_dir(game->map, vecf_scale(game->map->player.dir, -1));
	if (mlx_is_key_down(game->mlx, MLX_KEY_A))
		move_player_in_dir(game->map, vecf_scale(game->map->player.plane, -1));
	if (mlx_is_key_down(game->mlx, MLX_KEY_D))
		move_player_in_dir(game->map, game->map->player.plane);
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
