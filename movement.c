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

void	move_player_in_dir(t_map *map, t_v2f dir)
{
	t_v2f	pos;
	t_v2i	l;
	t_v2i	r;

	pos = vecf_add(map->player.position, vecf_scale(dir, MOVE_SPEED));
	l = veci_new(pos.x / CELL_SIZE, pos.y / CELL_SIZE);
	r = veci_new((pos.x + PLAYER_SIZE) / CELL_SIZE, (pos.y + PLAYER_SIZE)
			/ CELL_SIZE);
	if (!(l.y >= 0 && r.y < map->height && l.x >= 0 && r.x < map->width))
		return ;
	if (l.x >= (int32_t)ft_strlen(map->grid[l.y]))
		return ;
	if (r.x >= (int32_t)ft_strlen(map->grid[r.y]))
		return ;
	if (map->grid[l.y][l.x] == '0' && map->grid[l.y][r.x] == '0'
		&& map->grid[r.y][l.x] == '0' && map->grid[r.y][r.x] == '0')
		map->player.position = pos;
}

void	rotate_player(t_map *map, float_t angle)
{
	float_t	cos_a;
	float_t	sin_a;
	t_v2f	new_dir;
	t_v2f	new_plane;

	cos_a = cosf(angle);
	sin_a = sinf(angle);
	new_dir.x = map->player.dir.x * cos_a - map->player.dir.y * sin_a;
	new_dir.y = map->player.dir.x * sin_a + map->player.dir.y * cos_a;
	new_plane.x = map->player.plane.x * cos_a - map->player.plane.y * sin_a;
	new_plane.y = map->player.plane.x * sin_a + map->player.plane.y * cos_a;
	map->player.dir = new_dir;
	map->player.plane = new_plane;
}
