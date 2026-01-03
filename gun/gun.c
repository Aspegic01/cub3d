/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gun.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlabrirh <mlabrirh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/25 19:04:59 by klaayoun          #+#    #+#             */
/*   Updated: 2026/01/03 15:54:47 by mlabrirh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../cub3d.h"

static void	setup_gun_img(t_game *game, int i)
{
	int	x;
	int	y;

	x = (WIN_WIDTH - game->gun.frames[i]->width) / 2;
	y = WIN_HEIGHT - game->gun.frames[i]->height;
	mlx_image_to_window(game->mlx, game->gun.frames[i], x, y);
	if (i == 0)
		game->gun.frames[i]->instances[0].enabled = true;
	else
		game->gun.frames[i]->instances[0].enabled = false;
}

static void	init_gun_state(t_gun *gun)
{
	gun->curr_frame = 0;
	gun->is_active = false;
	gun->timer = 0.0;
}

static void	activate_gun(t_gun *gun)
{
	gun->is_active = true;
	gun->curr_frame = 0;
	gun->timer = 0;
}

int	init_gun(t_game *game)
{
	mlx_texture_t	*tex;
	int				i;
	char			path[100];

	i = 0;
	while (i < GUN_FRAMES)
	{
		make_gun_path(path, i + 1);
		tex = mlx_load_png(path);
		if (!tex)
		{
			ft_putstr_fd("Error: Could not load gun texture\n", 2);
			return (free_gun_frames(game, i), 1);
		}
		game->gun.frames[i] = mlx_texture_to_image(game->mlx, tex);
		mlx_delete_texture(tex);
		if (!game->gun.frames[i])
		{
			ft_putstr_fd("Error: Could not convert texture to image\n", 2);
			return (free_gun_frames(game, i), 1);
		}
		setup_gun_img(game, i);
		i++;
	}
	return (init_gun_state(&game->gun), 0);
}

void	update_gun(t_game *game)
{
	double	frame_time;
	t_gun	*gun;

	frame_time = 0.05;
	gun = &game->gun;
	if (mlx_is_key_down(game->mlx, MLX_KEY_R) && !gun->is_active)
		activate_gun(gun);
	if (gun->is_active)
	{
		gun->timer += game->mlx->delta_time;
		if (gun->timer >= frame_time)
		{
			gun->timer = 0;
			gun->frames[gun->curr_frame]->instances[0].enabled = false;
			gun->curr_frame++;
			if (gun->curr_frame >= GUN_FRAMES)
			{
				gun->curr_frame = 0;
				gun->is_active = false;
			}
			gun->frames[gun->curr_frame]->instances[0].enabled = true;
		}
	}
}
