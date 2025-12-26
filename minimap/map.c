/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: klaayoun <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/25 19:04:59 by klaayoun          #+#    #+#             */
/*   Updated: 2025/12/25 19:05:05 by klaayoun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../cub3d.h"

static void	render_cell(t_map *scene, t_v2i pos, uint32_t color)
{
	uint32_t	draw_color;
	uint32_t	x;
	uint32_t	y;

	y = 0;
	while (y < CELL_SIZE)
	{
		x = 0;
		while (x < CELL_SIZE)
		{
			draw_color = color;
			if (x == 0 || y == 0 || x == CELL_SIZE - 1 || y == CELL_SIZE - 1)
				draw_color = color << 1;
			mlx_put_pixel(scene->img, pos.x + x, pos.y + y, draw_color);
			x++;
		}
		y++;
	}
}

static t_v2i	clamp_view(t_v2i view, int width, int height)
{
	view.x = clamp(view.x, 0, width - 1);
	view.y = clamp(view.y, 0, height - 1);
	return (view);
}

static uint32_t	get_cell_color(t_map *map, t_v2i *mapPos, int line_len)
{
	if (mapPos->x < line_len && map->grid[mapPos->y][mapPos->x++] == '0')
		return (MAPFG);
	return (MAPBG);
}

void	render_moveable_cell(t_game *game, t_v2i *map_pos, t_v2i *view)
{
	t_v2i		cell;
	t_v2i		pos;
	uint32_t	color;
	int			line_len;

	cell = veci_zero();
	while (cell.y < CELL_COUNT && map_pos->y < game->map->height)
	{
		cell.x = 0;
		map_pos->x = view->x;
		line_len = ft_strlen(game->map->grid[map_pos->y]);
		while (cell.x < CELL_COUNT && map_pos->x < game->map->width)
		{
			color = get_cell_color(game->map, map_pos, line_len);
			pos = veci_new(cell.x * CELL_SIZE, cell.y * CELL_SIZE);
			render_cell(game->map, pos, color);
			cell.x++;
		}
		map_pos->y++;
		cell.y++;
	}
}

void	minimap_render(t_game *game)
{
	t_v2i	map_pos;
	t_v2i	view;

	map_pos = veci_fromf(game->map->player.position);
	view = veci_new(map_pos.x - CELL_COUNT_HALF, map_pos.y - CELL_COUNT_HALF);
	view = clamp_view(view, game->map->width, game->map->height);
	map_pos.y = view.y;
	render_moveable_cell(game, &map_pos, &view);
	render_player(game->map, 0x0000FFFF);
}
