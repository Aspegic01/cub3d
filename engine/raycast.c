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

void	draw_fov(t_game *game, t_map *scene, t_player *player)
{
	uint32_t x;
	float_t camerax;
	t_ray ray;

	x = 0;
	while (x < game->canvas->width)
	{
		camerax = 2.0 * (x / (float_t)game->canvas->width) - 1.0;
		ray = ft_init_ray(scene, camerax);
		int32_t line_height = ft_get_lineheight(ray.distance);
		int32_t draw_start = -line_height / 2 + game->canvas->height / 2;
		if (draw_start < 0)
			draw_start = 0;
		int32_t draw_end = line_height / 2 + game->canvas->height / 2;
		if (draw_end >= (int32_t)game->canvas->height)
			draw_end = game->canvas->height - 1;

		mlx_texture_t *tex = ft_get_texture(scene, ray.side, ray.step_dir);
		t_v2f hit_pos = vecf_add(player->position, vecf_scale(ray.dir, ray.distance));
		float wallX;
		if (ray.side == 0)
			wallX = hit_pos.y;
		else
			wallX = hit_pos.x;
		wallX -= floor(wallX);
		int texX = (int)(wallX * (float)tex->width);
		if (texX < 0)
			texX = 0;
		if (texX >= (int)tex->width)
			texX = tex->width - 1;

		float texStep = (float)tex->height / (float)line_height;
		float texPos = (draw_start - (float)game->canvas->height / 2 + (float)line_height / 2) * texStep;
		while (draw_start < draw_end)
		{
			int texY = (int)texPos;
			if (texY < 0)
				texY = 0;
			if (texY >= (int)tex->height)
				texY = tex->height - 1;
			int idx = (texY * tex->width + texX) * tex->bytes_per_pixel;
			uint8_t *p = &tex->pixels[idx];
			uint32_t pixcol = ft_get_pixel_color(p[0], p[1], p[2], p[3]);
			if (ray.side == 1)
				pixcol = ft_darken_color(pixcol);
			mlx_put_pixel(game->canvas, x, draw_start, pixcol);
			texPos += texStep;
			draw_start++;
		}
		x++;
	}
}
