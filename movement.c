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

bool	at_wall(t_map *scene, t_v2f pos)
{
	t_v2i	map_pos;

	map_pos = veci_new(pos.x / CELL_SIZE, pos.y / CELL_SIZE);
	if (map_pos.x >= (int32_t)ft_strlen(scene->grid[map_pos.y]))
		return (true);
	if (scene->grid[map_pos.y][map_pos.x] == '1')
		return (true);
	return (scene->grid[map_pos.y][map_pos.x] == '\0');
}

static bool	can_walk(t_map *map, int32_t x, int32_t y)
{
	if (y < 0 || y >= map->height || x < 0 || x >= map->width)
		return (false);
	if (x >= (int32_t)ft_strlen(map->grid[y]))
		return (false);
	return (map->grid[y][x] == '0');
}

void	move_player_in_dir(t_map *map, t_v2f dir)
{
	t_v2f	pos;
	t_v2i	l;
	t_v2i	r;
	bool	can_move_x;
	bool	can_move_y;

	pos = vecf_add(map->player.position, vecf_scale(dir, MOVE_SPEED));
	l = veci_new(pos.x / CELL_SIZE, pos.y / CELL_SIZE);
	r = veci_new((pos.x + PLAYER_SIZE) / CELL_SIZE, (pos.y + PLAYER_SIZE)
			/ CELL_SIZE);
	can_move_x = true;
	can_move_y = true;
	if (dir.x > 0)
		can_move_x = can_walk(map, r.x, l.y) && can_walk(map, r.x, r.y);
	else if (dir.x < 0)
		can_move_x = can_walk(map, l.x, l.y) && can_walk(map, l.x, r.y);
	if (dir.y > 0)
		can_move_y = can_walk(map, l.x, r.y) && can_walk(map, r.x, r.y);
	else if (dir.y < 0)
		can_move_y = can_walk(map, l.x, l.y) && can_walk(map, r.x, l.y);
	if (can_move_x && can_move_y)
		map->player.position = pos;
}

void	rotate_player(t_map *map, float_t angle)
{
	map->player.dir = vecf_rot(map->player.dir, angle);
	map->player.plane = vecf_rot(map->player.plane, angle);
}
