/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycast.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: klaayoun <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/25 19:22:56 by klaayoun          #+#    #+#             */
/*   Updated: 2025/12/25 19:22:59 by klaayoun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../cub3d.h"

void	ft_compute_tex(t_wall_texture *wall, t_map *scene,
		t_wall_stripe *stripe)
{
	wall->tex = ft_get_texture(scene, stripe->ray.side, stripe->ray.step_dir);
	if (stripe->ray.side == 0)
		wall->wallx = stripe->ray.hit_pos.y;
	else
		wall->wallx = stripe->ray.hit_pos.x;
	wall->wallx -= floor(wall->wallx);
	wall->texx = clamp(wall->wallx * (float)wall->tex->width, 0,
			wall->tex->width - 1);
	wall->tex_step = (float_t)wall->tex->height / (float_t)stripe->height;
	wall->tex_pos = (stripe->start - (float_t)WIN_HEIGHT / 2
			+ (float_t)stripe->height / 2) * wall->tex_step;
}

void	ft_apply_texture(t_game *game, int32_t x, t_wall_texture *wall,
		t_wall_stripe *stripe)
{
	int32_t		texy;
	int			idx;
	uint8_t		*p;
	uint32_t	pixcol;

	texy = clamp(wall->tex_pos, 0, wall->tex->height - 1);
	idx = (texy * wall->tex->width + wall->texx) * wall->tex->bytes_per_pixel;
	p = &wall->tex->pixels[idx];
	pixcol = ft_get_pixel_color(p[0], p[1], p[2], p[3]);
	if (stripe->ray.side == 1)
		pixcol = ft_darken_color(pixcol);
	mlx_put_pixel(game->canvas, x, stripe->start, pixcol);
	wall->tex_pos += wall->tex_step;
	stripe->start++;
}

void	draw_fov(t_game *game, t_map *scene)
{
	uint32_t		x;
	float_t			camerax;
	t_wall_stripe	stripe;
	t_wall_texture	wall;

	x = 0;
	while (x < game->canvas->width)
	{
		camerax = 2.0 * (x / (float_t)game->canvas->width) - 1.0;
		stripe = ft_compute_wall_stripe(scene, camerax);
		ft_compute_tex(&wall, scene, &stripe);
		while (stripe.start < stripe.end)
		{
			ft_apply_texture(game, x, &wall, &stripe);
		}
		x++;
	}
}
