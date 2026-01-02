/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: klaayoun <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/25 19:22:56 by klaayoun          #+#    #+#             */
/*   Updated: 2025/12/25 19:22:59 by klaayoun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../cub3d.h"

static int	get_rgba(int r, int g, int b, int a)
{
	return (r << 24 | g << 16 | b << 8 | a);
}

void	render_floor(t_game *game)
{
	uint32_t	boundary;
	uint32_t	x;
	uint32_t	y;
	uint32_t	c;

	y = 0;
	boundary = game->canvas->height / 2;
	while (y < boundary)
	{
		x = 0;
		while (x < game->canvas->width)
		{
			c = get_rgba(game->map->colors.ceiling_r,
					game->map->colors.ceiling_g, game->map->colors.ceiling_b,
					255);
			mlx_put_pixel(game->canvas, x, y, c);
			x++;
		}
		y++;
	}
}

void	render_ceiling(t_game *game)
{
	uint32_t	x;
	uint32_t	y;
	uint32_t	c;

	y = game->canvas->height / 2;
	while (y < game->canvas->height)
	{
		x = 0;
		while (x < game->canvas->width)
		{
			c = get_rgba(game->map->colors.floor_r, game->map->colors.floor_g,
					game->map->colors.floor_b, 255);
			mlx_put_pixel(game->canvas, x, y, c);
			x++;
		}
		y++;
	}
}

void	render_game(t_game *game)
{
	render_ceiling(game);
	render_floor(game);
	draw_fov(game, game->map);
	minimap_render(game);
	update_gun(game);
}
