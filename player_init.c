/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   player_init.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlabrirh <mlabrirh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/28 13:10:00 by mlabrirh          #+#    #+#             */
/*   Updated: 2025/08/28 13:57:43 by mlabrirh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"


	/*
	 dir x negative we looking left;
	 dir x positive we looking right;
	 dir y posivite we looking down
	 dir y negative we looking up
	 * */
void	set_player_direction(t_player *player, char direction)
{
	if (direction == 'N')
	{
		player->dir = vecf_new(0, -1);
		player->plane = vecf_new(0.66, 0);
	}
	else if (direction == 'S')
	{
		player->dir = vecf_new(0, 1);
		player->plane = vecf_new(-0.66, 0);
	}
	else if (direction == 'E')
	{
		player->dir = vecf_new(1, 0);
		player->plane = vecf_new(0, 0.66);
	}
	else if (direction == 'W')
	{
		player->dir = vecf_new(-1, 0);
		player->plane = vecf_new(0, -0.66);
	}
}

int	find_player_position(t_map *map, t_player *player)
{
	int		i;
	int		j;
	char	direction;
	int		player_count;

	i = 0;
	player_count = 0;
	while (i < map->height)
	{
		j = 0;
		while (j < (int)ft_strlen(map->grid[i]))
		{
			if (map->grid[i][j] == 'N' || map->grid[i][j] == 'S' || 
				map->grid[i][j] == 'E' || map->grid[i][j] == 'W')
			{
				direction = map->grid[i][j];
				player->position = vecf_new(j, i);
				map->grid[i][j] = '0';
				set_player_direction(player, direction);
				player_count++;
			}
			j++;
		}
		i++;
	}
	return (player_count);
}

int	init_player(t_map *map, t_player *player)
{
	int	player_count;

	player_count = find_player_position(map, player);
	if (player_count == 0)
		return (ft_putstr_fd("Error\nNo player starting position found\n", 2), -1);
	if (player_count > 1)
		return (ft_putstr_fd("Error\nMultiple player positions found\n", 2), -1);
	// printf("Player initialized successfully!\n");
	// printf("Position: (%.1f, %.1f)\n", player->x, player->y);
	// printf("Direction: (%.2f, %.2f)\n", player->dir_x, player->dir_y);
	// printf("Camera plane: (%.2f, %.2f)\n", player->plane_x, player->plane_y);
	
	return (0);
}
