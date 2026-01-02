/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gun.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: klaayoun <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/25 19:04:59 by klaayoun          #+#    #+#             */
/*   Updated: 2025/12/25 19:05:05 by klaayoun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../cub3d.h"

static void	make_gun_path(char *buffer, int i)
{
	char	*prefix;
	int		j;
	char	*suffix;
	int		k;

	prefix = "textures/shitgun_animation/shitgun_animation_";
	j = 0;
	while (prefix[j])
	{
		buffer[j] = prefix[j];
		j++;
	}
	if (i < 10)
		buffer[j++] = i + '0';
	else
	{
		buffer[j++] = (i / 10) + '0';
		buffer[j++] = (i % 10) + '0';
	}
	suffix = ".png";
	k = 0;
	while (suffix[k])
		buffer[j++] = suffix[k++];
	buffer[j] = '\0';
}

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
			return (1);
		}
		game->gun.frames[i] = mlx_texture_to_image(game->mlx, tex);
		mlx_delete_texture(tex);
		setup_gun_img(game, i);
		i++;
	}
	game->gun.curr_frame = 0;
	game->gun.is_active = false;
	game->gun.timer = 0.0;
	return (0);
}

void	update_gun(t_game *game)
{
	double	frame_time;

	frame_time = 0.05;
	if (mlx_is_key_down(game->mlx, MLX_KEY_R) && !game->gun.is_active)
	{
		game->gun.is_active = true;
		game->gun.curr_frame = 0;
		game->gun.timer = 0;
	}
	if (game->gun.is_active)
	{
		game->gun.timer += game->mlx->delta_time;
		if (game->gun.timer >= frame_time)
		{
			game->gun.timer = 0;
			game->gun.frames[game->gun.curr_frame]->instances[0].enabled = false;
			game->gun.curr_frame++;
			if (game->gun.curr_frame >= GUN_FRAMES)
			{
				game->gun.curr_frame = 0;
				game->gun.is_active = false;
			}
			game->gun.frames[game->gun.curr_frame]->instances[0].enabled = true;
		}
	}
}
