/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   movement.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlabrirh <mlabrirh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/04 11:30:00 by mlabrirh          #+#    #+#             */
/*   Updated: 2025/10/17 11:23:23 by mlabrirh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static bool	can_walk(t_map *map, int32_t x, int32_t y)
{
	if (y < 0 || y >= map->height || x < 0 || x >= map->width)
		return (false);
	if (x >= (int32_t)ft_strlen(map->grid[y]))
		return (false);
	return (map->grid[y][x] == '0');
}

static bool	is_valid_move(t_map *map, t_v2f pos, t_v2f dir, bool check_x)
{
	t_v2i	l;
	t_v2i	r;

	l = veci_new(pos.x / CELL_SIZE, pos.y / CELL_SIZE);
	r = veci_new((pos.x + PLAYER_SIZE) / CELL_SIZE, (pos.y + PLAYER_SIZE)
			/ CELL_SIZE);
	if (check_x)
	{
		if (dir.x > 0)
			return (can_walk(map, r.x, l.y) && can_walk(map, r.x, r.y));
		if (dir.x < 0)
			return (can_walk(map, l.x, l.y) && can_walk(map, l.x, r.y));
	}
	if (dir.y > 0)
		return (can_walk(map, l.x, r.y) && can_walk(map, r.x, r.y));
	if (dir.y < 0)
		return (can_walk(map, l.x, l.y) && can_walk(map, r.x, l.y));
	return (true);
}

void	move_player_in_dir(t_map *map, t_v2f dir)
{
	t_v2f	new_pos;

	new_pos.x = map->player.position.x + (dir.x * MOVE_SPEED);
	new_pos.y = map->player.position.y;
	if (is_valid_move(map, new_pos, dir, true))
	{
		map->player.position.x = new_pos.x;
		map->run = true;
	}
	new_pos.x = map->player.position.x;
	new_pos.y = map->player.position.y + (dir.y * MOVE_SPEED);
	if (is_valid_move(map, new_pos, dir, false))
	{
		map->player.position.y = new_pos.y;
		map->run = true;
	}
}

void	rotate_player(t_map *map, float_t angle)
{
	map->player.dir = vecf_rot(map->player.dir, angle);
	map->player.plane = vecf_rot(map->player.plane, angle);
	map->run = true;
}
