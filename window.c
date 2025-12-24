/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   window.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlabrirh <mlabrirh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/04 11:30:00 by mlabrirh          #+#    #+#             */
/*   Updated: 2025/12/19 17:31:57 by mlabrirh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

int	close_window(t_game *game)
{
	if (game->mlx)
		mlx_close_window(game->mlx);
	return (0);
}

void	mouse_callback(double xpos, double ypos, void *param)
{
	t_game	*game;
	double	xoffset;

	game = param;
	if (game->first_mouse)
	{
		game->last_mouse_x = (int32_t)xpos;
		game->last_mouse_y = (int32_t)ypos;
		game->first_mouse = false;
		return;
	}
	xoffset = xpos - game->last_mouse_x;
	game->last_mouse_x = (int32_t)xpos;
	game->last_mouse_y = (int32_t)ypos;
	rotate_player(game, xoffset * MOUSE_SENSITIVITY);
}

void	cursor_handler(double xpos, double ypos, void *param)
{
	t_game	*game;
	double	xoffset;
	int		margin;
	static bool	repositioning = false;

	(void)ypos;
	game = (t_game *)param;
	if (game->pause_game || repositioning)
		return ;
	if (game->first_mouse)
	{
		game->last_mouse_x = (int32_t)xpos;
		game->last_mouse_y = (int32_t)ypos;
		game->first_mouse = false;
		return ;
	}
	
	margin = 50;
	// Check if we need to reposition before processing movement
	if (xpos < margin || xpos > WIN_WIDTH - margin)
	{
		repositioning = true;
		if (xpos < margin)
		{
			mlx_set_mouse_pos(game->mlx, WIN_WIDTH - margin - 1, (int)ypos);
			game->last_mouse_x = WIN_WIDTH - margin - 1;
		}
		else
		{
			mlx_set_mouse_pos(game->mlx, margin + 1, (int)ypos);
			game->last_mouse_x = margin + 1;
		}
		repositioning = false;
		return ;
	}
	
	xoffset = xpos - game->last_mouse_x;
	game->last_mouse_x = (int32_t)xpos;
	game->last_mouse_y = (int32_t)ypos;
	rotate_player(game, xoffset * MOUSE_SENSITIVITY);
}

void	capture_keys(void *param)
{
	t_game	*game;

	game = param;
	if (mlx_is_key_down(game->mlx, MLX_KEY_ESCAPE))
		close_window(game);
	if (mlx_is_key_down(game->mlx, MLX_KEY_W))
		move_player_in_dir(game, game->map->player.dir);
	if (mlx_is_key_down(game->mlx, MLX_KEY_S))
		move_player_in_dir(game, vecf_scale(game->map->player.dir, -1));
	if (mlx_is_key_down(game->mlx, MLX_KEY_A))
		move_player_in_dir(game, vecf_scale(game->map->player.plane, -1));
	if (mlx_is_key_down(game->mlx, MLX_KEY_D))
		move_player_in_dir(game, game->map->player.plane);
	if (mlx_is_key_down(game->mlx, MLX_KEY_LEFT))
		rotate_player(game, -ROT_SPEED * game->mlx->delta_time);
	if (mlx_is_key_down(game->mlx, MLX_KEY_RIGHT))
		rotate_player(game, ROT_SPEED * game->mlx->delta_time);
}

static void	start(void *param)
{
	t_game	*game;

	game = param;
	render_game(game);
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
	if (minimap_setup(game) != 0)
		return (-1);
	// Initialize mouse tracking
	game->first_mouse = true;
	game->last_mouse_x = WIN_WIDTH / 2;
	game->last_mouse_y = WIN_HEIGHT / 2;
	game->pause_game = false;
	game->player_angle = 0.0;
	// Set cursor to hidden for FPS-style camera
	mlx_set_cursor_mode(game->mlx, MLX_MOUSE_HIDDEN);
	// Set up hooks
	mlx_cursor_hook(game->mlx, cursor_handler, game);
	mlx_loop_hook(game->mlx, capture_keys, game);
	mlx_loop_hook(game->mlx, start, game);
	return (0);
}
