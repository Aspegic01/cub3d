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

void	render_player(t_map *scene, uint32_t color)
{
	t_v2f	player_pos;
	t_v2i	grid_pos;
	t_v2i	pixel;
	t_v2i	offset;

	grid_pos = veci_new(scene->player.position.x - CELL_COUNT_HALF, scene->player.position.y - CELL_COUNT_HALF);
	grid_pos.x = clamp(grid_pos.x, 0, scene->width - 1);
	grid_pos.y = clamp(grid_pos.y, 0, scene->height - 1);
	player_pos.x = (scene->player.position.x - grid_pos.x) * CELL_SIZE;
	player_pos.y = (scene->player.position.y - grid_pos.y) * CELL_SIZE;
	offset.y = 0;
	while (offset.y < (int32_t)PLAYER_SIZE)
	{
		offset.x = 0;
		while (offset.x < (int32_t)PLAYER_SIZE)
		{
			pixel = veci_new(player_pos.x + offset.x, player_pos.y + offset.y);
			if (pixel.x >= 0 && pixel.x < scene->size.x && pixel.y >= 0 && pixel.y < scene->size.y)
				mlx_put_pixel(scene->img, pixel.x, pixel.y, color);
			offset.x++;
		}
		offset.y++;
	}
	draw_dirline(scene, player_pos, color);
}
