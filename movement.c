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

bool	is_valid_move(t_map *map, t_v2f pos, t_v2f dir, bool check_x)
{
	t_v2f	l;
	t_v2f	r;

	l = vecf_new(pos.x * CELL_SIZE, pos.y * CELL_SIZE);
	r = vecf_new(l.x + PLAYER_SIZE, l.y + PLAYER_SIZE);
	l = vecf_scale(l, (1.0 / CELL_SIZE));
	r = vecf_scale(r, (1.0 / CELL_SIZE));
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

void	move_player_in_dir(t_game *game, t_v2f dir)
{
	t_v2f	new_pos;
	float_t	speedx;
	float_t	speedy;

	speedx = (dir.x * MOVE_SPEED) * game->mlx->delta_time;
	new_pos.x = game->map->player.position.x + speedx;
	new_pos.y = game->map->player.position.y;
	if (is_valid_move(game->map, new_pos, dir, true))
		game->map->player.position.x = new_pos.x;
	speedy = (dir.y * MOVE_SPEED) * game->mlx->delta_time;
	new_pos.x = game->map->player.position.x;
	new_pos.y = game->map->player.position.y + speedy;
	if (is_valid_move(game->map, new_pos, dir, false))
		game->map->player.position.y = new_pos.y;
}

void	rotate_player(t_game *game, float_t rot_speed)
{
	float_t	speed;

	speed = rot_speed * game->mlx->delta_time;
	game->map->player.dir = vecf_rot(game->map->player.dir, speed);
	game->map->player.plane = vecf_rot(game->map->player.plane, speed);
}
