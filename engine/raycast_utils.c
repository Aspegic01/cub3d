/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycast_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: klaayoun <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/25 19:22:56 by klaayoun          #+#    #+#             */
/*   Updated: 2025/12/25 19:22:59 by klaayoun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../cub3d.h"

bool	ft_at_wall(t_map *map, float offsetx, float offsety)
{
	int32_t	x;
	int32_t	y;

	x = (int)offsetx;
	y = (int)offsety;
	if (y <= 0 || y >= map->height || x <= 0 || x >= map->width)
		return (true);
	if (x >= (int32_t)ft_strlen(map->grid[y]))
		return (true);
	return (map->grid[y][x] != '0');
}

mlx_texture_t	*ft_get_texture(t_map *scene, uint32_t side, t_v2f step)
{
	if (side == 0)
	{
		if (step.x < 0)
			return (scene->tex_west);
		return (scene->tex_east);
	}
	if (step.y < 0)
		return (scene->tex_north);
	return (scene->tex_south);
}

uint32_t	ft_get_pixel_color(uint8_t r, uint8_t g, uint8_t b, uint8_t a)
{
	return ((uint32_t)r << 24 | (uint32_t)g << 16 | (uint32_t)b << 8 | a);
}

uint32_t	ft_darken_color(uint32_t color)
{
	uint8_t	r;
	uint8_t	g;
	uint8_t	b;
	uint8_t	a;

	r = (color >> 24) & 0xFF;
	g = (color >> 16) & 0xFF;
	b = (color >> 8) & 0xFF;
	a = color & 0xFF;
	return (ft_get_pixel_color(r / 2, g / 2, b / 2, a));
}

int32_t	ft_get_lineheight(float distance)
{
	int32_t	line_height;

	if (distance == 0)
		line_height = WIN_HEIGHT;
	else
		line_height = WIN_HEIGHT / distance;
	return (line_height);
}
