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

static void	move_player_in_dir(t_map *map, double move_x, double move_y)
{
	int32_t	box;
	double	new_x;
	double	new_y;
	int32_t	grid_x1, grid_x2, grid_y1, grid_y2;
	double	move_speed;

	move_speed = 2.0; 
	box = map->cell_size * 0.5;
	
	new_x = map->player.x + (move_x * move_speed);
	new_y = map->player.y + (move_y * move_speed);
	
	grid_x1 = new_x / map->cell_size;
	grid_x2 = (new_x + box) / map->cell_size;
	grid_y1 = new_y / map->cell_size;
	grid_y2 = (new_y + box) / map->cell_size;
	
	if (grid_y1 >= 0 && grid_y2 < map->height && 
	    grid_x1 >= 0 && grid_x2 < map->width)
	{
		int line_len1 = ft_strlen(map->grid[grid_y1]);
		int line_len2 = ft_strlen(map->grid[grid_y2]);
		
		if (grid_x1 < line_len1 && grid_x2 < line_len2 &&
		    map->grid[grid_y1][grid_x1] == '0' && 
		    map->grid[grid_y1][grid_x2] == '0' &&
		    map->grid[grid_y2][grid_x1] == '0' && 
		    map->grid[grid_y2][grid_x2] == '0')
		{
			map->player.x = new_x;
			map->player.y = new_y;
		}
	}
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
	if (mlx_is_key_down(game->mlx, MLX_KEY_W))
		move_player_in_dir(game->map, game->map->player.dir_x, game->map->player.dir_y);
	if (mlx_is_key_down(game->mlx, MLX_KEY_S))
		move_player_in_dir(game->map, -game->map->player.dir_x, -game->map->player.dir_y);
	if (mlx_is_key_down(game->mlx, MLX_KEY_A))
		move_player_in_dir(game->map, -game->map->player.plane_x, -game->map->player.plane_y);
	if (mlx_is_key_down(game->mlx, MLX_KEY_D))
		move_player_in_dir(game->map, game->map->player.plane_x, game->map->player.plane_y);
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
