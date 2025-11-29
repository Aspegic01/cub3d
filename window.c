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

void	capture_keys(void *param)
{
	t_game	*game;

	game = param;
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
		rotate_player(game->map, -ROT_SPEED);
	if (mlx_is_key_down(game->mlx, MLX_KEY_RIGHT))
		rotate_player(game->map, ROT_SPEED);
}

static void	start(void *param)
{
	t_game	*game;

	game = param;
	if (!game->map->run)
		return;
	render_game(game);
	game->map->run = false;
}

int	init_window(t_game *game)
{
	game->mlx = mlx_init(WIN_WIDTH, WIN_HEIGHT, WIN_TITLE, false);
	if (!game->mlx)
		return (ft_putstr_fd("Error\nFailed to initialize MLX\n", 2), -1);
	game->canvas = mlx_new_image(game->mlx, game->mlx->width,
			game->mlx->height);
	if (!game->canvas)
		return (ft_putstr_fd("Error\nFailed to init image\n", 2), -1);
	if (mlx_image_to_window(game->mlx, game->canvas, 0, 0) < 0)
		return (ft_putstr_fd("Error\nFailed to put image to window\n", 2), -1);
	game->map->run = true;
	if (minimap_setup(game) != 0)
		return (-1);
	mlx_loop_hook(game->mlx, capture_keys, game);
	mlx_loop_hook(game->mlx, start, game);
	return (0);
}
