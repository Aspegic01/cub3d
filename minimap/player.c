/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   player.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: klaayoun <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/25 19:04:59 by klaayoun          #+#    #+#             */
/*   Updated: 2025/12/25 19:05:05 by klaayoun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../cub3d.h"

void	draw_dirline(t_map *scene, t_v2f player_pos, uint32_t color)
{
	t_v2f	direction;

	player_pos.x += PLAYER_HALF;
	player_pos.y += PLAYER_HALF;
	direction = vecf_scale(scene->player.dir, (float_t)CELL_SIZE / 1.5);
	draw_line(scene->img, player_pos, vecf_add(player_pos, direction), color);
}

static t_v2i	clamp_view(t_v2i view, int width, int height)
{
	view.x = clamp(view.x, 0, width - 1);
	view.y = clamp(view.y, 0, height - 1);
	return (view);
}

void	draw_player(t_map *scene, t_v2f *pos, uint32_t color)
{
	t_v2i	offset;
	t_v2i	pixel;

	offset = veci_zero();
	while (offset.y < (int32_t)PLAYER_SIZE)
	{
		offset.x = 0;
		while (offset.x < (int32_t)PLAYER_SIZE)
		{
			pixel = veci_new(pos->x + offset.x, pos->y + offset.y);
			if (pixel.x >= 0 && pixel.x < scene->size.x && pixel.y >= 0
				&& pixel.y < scene->size.y)
				mlx_put_pixel(scene->img, pixel.x, pixel.y, color);
			offset.x++;
		}
		offset.y++;
	}
}

void	render_player(t_map *scene, uint32_t color)
{
	t_v2f	player_pos;
	t_v2i	map_pos;
	t_v2i	view;

	map_pos = veci_fromf(scene->player.position);
	view = veci_new(map_pos.x - CELL_COUNT_HALF, map_pos.y - CELL_COUNT_HALF);
	view = clamp_view(view, scene->width, scene->height);
	player_pos.x = (scene->player.position.x - view.x) * CELL_SIZE;
	player_pos.y = (scene->player.position.y - view.y) * CELL_SIZE;
	draw_player(scene, &player_pos, color);
	draw_dirline(scene, player_pos, color);
}
